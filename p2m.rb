# -*- mode: ruby; encoding: utf-8 -*-
require 'uri'
require 'date'
require 'yaml'
require 'kconv'


#
# pukiwikiのデータをMarkDownにする.
# 使い方:
# $ ruby -E UTF-8 p2m.rb ./wiki ./md
# 注意:
# wiki/*.txt はUTF-8にしておいてください.
#

module HTMLUtils
  ESC = {
    '&' => '&amp;',
    '"' => '&quot;',
    '<' => '&lt;',
    '>' => '&gt;'
  }
  def escape(str)
    table = ESC   # optimize
    str.gsub(/[&"<>]/u) {|s| table[s]}
  end
  CODE = {
    '<' => '&lt;',
    '>' => '&gt;',
    '&' => '&amp;'
  }
  def code_escape(str)
    table = CODE
    str.gsub(/[<>&]/u) {|s| table[s]}
  end
  URIENC = {
    '(' => '%28',
    ')' => '%29',
    ' ' => '%20'
  }
  def uri_encode(str)
    table = URIENC
    str.gsub(/[\(\) ]/u) {|s| table[s]}
  end
  def urldecode(str)
    str.gsub(/[A-F\d]{2}/u) {|x| [x.hex].pack('C*')}
  end
end

class PukiWikiParser
  include HTMLUtils

  def initialize()
    @h_start_level = 2
  end
  def filename(pw_name)

# 小文字にしない.
#    decoded_name = HTMLUtils.urldecode(pw_name).sub(/\:/, '_').downcase.split("/").last
    decoded_name = HTMLUtils.urldecode(pw_name).sub(/\:/, '_').split("/").last
    name = decoded_name.sub(/\.txt$/, '.md')
	name = name.toutf8

    if @timestamp.nil? || @timestamp.size===0
      return name
    else
      return "#{@timestamp}-#{name}"
    end
  end
  def has_pubdate
    @timestamp != ''
  end

#
# $$$ 変換
#
  def to_md(src, page_names, page, base_uri = 'http://psp.dip.jp/X/', suffix= '/')
    @page_names = page_names
    @base_uri = base_uri
    @page = page.sub(/\ASwing\/(.+)\.txt$/) { $1 }
    @pagelist_suffix = suffix
    @inline_re = nil

    @timestamp = ''

    head = []
    buf = []

    @FRONT_MATTER_REGEX ||= %r<
      \A---[\r\n](.*?)[\r\n]---[\r\n](.*)
    >mx
    if @FRONT_MATTER_REGEX =~ src.lstrip then
      frontmatter = $1
      body = $2

#       yaml = YAML.load(frontmatter)
#       yaml['layout']   = 'post'
#       yaml['category'] = 'swing'
#       yaml['folder']   = @page
#       yaml['comments'] = true
#       if yaml.key?('noindex') then
#         return ''
#       end
#       @timestamp = yaml['pubdate'].strftime('%Y-%m-%d')
#       head.push(yaml.to_yaml.rstrip)

      head.push("---")
      head.push("layout: post")
      head.push("category: swing")
      head.push("folder: #{@page}")
      heads = frontmatter.rstrip.split(/\r?\n/).map {|line| line.chomp }
      while heads.first
        case heads.first
        when /\Apubdate: /
          pubdate = heads.shift
          head.push pubdate
          @timestamp = DateTime.parse(pubdate.sub(/\Apubdate: /, '')).strftime('%Y-%m-%d')
        when /\Anoindex: /
          @timestamp = ''
          return ''
        else
          head.push heads.shift
        end
      end
      head.push("comments: true")
      head.push("---\n")
    else
      body = src
    end

    lines = body.rstrip.split(/\r?\n/).map {|line| line.chomp }
    while lines.first
      case lines.first
      when ''
        buf.push lines.shift
      when /\A----/
        lines.shift
        buf.push '- - - -' #hr
      when /\A\*/
        buf.push parse_h(lines.shift)
      when /\A\#code.*\{\{/
        buf.concat parse_pre2(take_multi_block(lines))
      when /\A\#.+/
        bp = parse_block_plugin(lines.shift)
        buf.push bp unless bp.nil?
      when /\A\s/
        buf.concat parse_pre(take_block(lines, /\A\s/))
      when /\A\/\//
        #buf.concat parse_comment(take_block(lines, /\A\/\//))
        take_block(lines, /\A\/\//)
      when /\A>/
        buf.concat parse_quote(take_block(lines, /\A>/))
      when /\A-/
        buf.concat parse_list('ul', take_list_block(lines))
      when /\A\+/
        buf.concat parse_list('ol', take_block(lines, /\A\+/))
      when /\A:/
        buf.concat parse_dl(take_block(lines, /\A:/))
      else
        buf.concat parse_p(take_block(lines, /\A(?![*\s>:\-\+\#]|----|\z)/))
      end
    end
    buf.join("\n")

    head.join("\n").concat(buf.join("\n"))
  end

  private

  def take_block(lines, marker)
    buf = []
    until lines.empty?
      break unless marker =~ lines.first
      if /\A\/\// =~ lines.first then
        lines.shift
      else
        buf.push lines.shift.sub(marker, '')
      end
    end
    buf
  end

  def take_multi_block(lines)
    buf = []
    until lines.empty?
      l = lines.shift
      break if /^\}\}$/ =~ l
      next  if /^.code.*$/ =~ l
      buf.push l
    end
    buf
  end

  def parse_h(line)
    level = @h_start_level + (line.slice(/\A\*{1,4}/).length - 1)
    h = "#"*level
    # content = line.sub(/\A\*+/, '')
    ##content = line.gsub(/\A\*+(.+) \[#\w+\]$/) { $1 }
    #"<h#{level}>#{parse_inline(content)}</h#{level}>"
    content = line.gsub(/\A\*+(.+)$/) { $1.gsub(/ +\[#\w+\]$/, "") }
    "#{h} #{parse_inline(content).strip}"
  end

  def take_list_block(lines)
    marker = /\A-/
    buf = []
    codeblock = false
    listblock = false
    until lines.empty?
      #break unless marker =~ lines.first
      #while lines.first
      case lines.first
      when /\A\/\//
        lines.shift
      when /\A----/
        if codeblock then
          buf.push "<!-- dummy comment line for breaking list -->"
        end
        #buf.push "<!-- dummy comment line for breaking list -->"
        break
      when marker
        l = lines.shift
        #puts l
        buf.push l #lines.shift #.sub(marker, '')
        listblock = true
        codeblock = false
        #puts buf.last
#       when /\A$/
#         buf.push lines.shift
      when /\A\s/
        buf.push '#' + lines.shift
        codeblock = true
        listblock = false
      when /\A\#code.*\{\{/
        array = []
        until lines.empty?
          l = lines.shift
          array.push l
          break if /^\}\}$/ =~ l
        end
        buf.concat array
        codeblock = true
        listblock = false
      else
        if listblock then
          buf.push "<!-- dummy comment line for breaking list -->"
          break
        elsif codeblock then
          buf.push lines.shift
        else
          break
        end
      end
    end
    buf
  end

  def parse_list(type, lines)
    marker = ((type == 'ul') ? /\A-+/ : /\A\++/)
    parse_list0(type, lines, marker)
  end

  def parse_list0(type, lines, marker)
    buf = []
    level = 0
    blockflag = false
    until lines.empty?
      line = lines.shift.strip
      aaa = line.slice(marker)
      if aaa then
        level = aaa.length - 1
        line = line.sub(marker,'').strip
      #else
      #  level = 0
      end
      h = "    "*level
      s = (type == 'ul') ? '-' : '1.'

      if line.empty? then
        #buf.push line
      elsif line.start_with?('#code') then
        hh = "    "*(level+1)
        array = take_multi_block(lines).map{|ll| hh + code_escape(ll)}
        line = array.shift.strip
        buf.concat [hh, %Q|#{hh}<pre class="prettyprint"><code>|.concat(line), array.join("\n"), "</code></pre>"]
        blockflag = false
      elsif line.start_with?('#') then
        unless blockflag then
          blockflag = true
          buf.push h
        end
        x = "\t"*2
        line = code_escape(line.sub(/\A\#\s/, ''))
        buf.push "#{h}#{x}#{line}"
      elsif  line.start_with?('<!--') then
        buf.concat ['', line]
        break
      else
        blockflag = false
        #puts "#{level}: #{line}"
        buf.push "#{h}#{s} #{parse_inline(line)}"
      end
    end
    buf
  end

  def parse_dl(lines)
    buf = ["<dl>"]
    lines.each do |line|
      dt, dd = *line.split('|', 2)
      buf.push "<dt>#{parse_inline(dt)}</dt>"
      buf.push "<dd>#{parse_inline(dd)}</dd>" if dd
    end
    buf.push "</dl>"
    buf
  end

  def parse_quote(lines)
    ["<blockquote><p>", lines.join("\n"), "</p></blockquote>"]
  end

  def parse_pre(lines)
    #[%Q|#{lines.map {|line| "\t".concat(line) }.join("\n")}|, %Q|{:class="prettyprint"}|]
    lines.map{|line| "\t".concat(line)} #.join("\n")
  end

  def parse_pre2(lines)
    array = lines.map{|line| code_escape(line)}
    array[0] = %Q|<pre class="prettyprint"><code>|.concat(array[0])
    [array.join("\n"), "</code></pre>"]
  end

  def parse_pre3(lines)
    ["```java", lines.join("\n"), "```"]
  end

  def parse_comment(lines)
    ["<!-- #{lines.map {|line| escape(line) }.join("\n")}",
      ' -->']
  end

  def parse_p(lines)
    lines.map {|line| parse_inline(line)}
  end

  def parse_inline(str)
    str = str.gsub(/%%(?!%)((?:(?!%%).)*)%%/) { ['~~', $1, '~~'].join() } #nest: <del>[http://example.com/ example]</del>, <strike>
    @inline_re ||= %r~
    &(?<plugin>[0-9A-Za-z_]+)(?:\((?<parameter>[^\)]*)\))?(?:{(?<inline>[^}]+)})?; # inline plugin ex. &new(...){...};
      | \[\[(?<bracket>[^>]+)>(?<uri>[^\]]*)\]\]   # bracket, URI
      | \[\[(?<bracket>[^:]+):(?<uri>[^\]]*)\]\]   # bracket, URI
      | \[\[(?<blabel>[^\]]+)\]\]                   # blabel
      | \[(?<uri>https?://\S+)\s+(?<label>[^\]]+)\] # URI, label
      | (?<uri>#{URI.regexp('http')})               # URI autolink
      | KBD\{(?<kbd>[^\}]+)\}                       # <kbd>
      | ``(?!`)(?<code>(?:(?!``).)*)``              # <code>
      | \'\'(?!\')(?<strong>(?:(?!\'\').)*)\'\'     # <strong>
      #| %%(?!%)(?<del>(?:(?!%%).)*)%%               # <del>, <strike>
    ~x
    str.gsub(@inline_re) {
      case
      when $~[:plugin]  then parse_inline_plugin($~)
      when $~[:bracket] then a_href($~[:uri].strip, $~[:bracket], 'pagelink')
      when $~[:blabel]  then a_href(''            , $~[:blabel], 'pagelink')
      when $~[:label]   then a_href($~[:uri].strip, $~[:label], '')
      when $~[:uri]     then a_href($~[:uri].strip, $~[:uri], '')
      when $~[:kbd]     then ['<kbd>', $~[:kbd], '</kbd>'].join()
      when $~[:code]    then ['<code>', $~[:code], '</code>'].join()
      when $~[:strong]  then ['**', $~[:strong], '**'].join()
      #when $~[:del]     then ['~~', $~[:del], '~~'].join()
      else
        raise 'must not happen'
      end
    }
  end

  def parse_inline_plugin(mtch) #plugin, para, inline)
    plugin = mtch[:plugin].strip
    parameter = mtch[:parameter]
    inline = mtch[:inline]
    case plugin
    when 'jnlp'
      %Q|{% jnlp %}|
    when 'jar2'
      %Q|{% jar %}|
    when 'zip'
      %Q|{% src %}\n- {% svn %}|
    when 'ref'
      parameter
    when 'new'
      inline.strip
    when 'user'
      %Q|*#{parameter}*|
    else
      plugin
    end
  end

  def parse_block_plugin(line)
    @plugin_re = %r<
        \A\#([^\(]+)\(?([^\)]*)\)?
      >x
    args = []
    line.gsub(@plugin_re) {
      args.push $1
      args.push $2 #.slice(",")
    }
    #buf = []
    case args.first
    when 'download'
      %Q<{% download #{args[1]} %}>
    when 'ref'
      %Q<![screenshot](#{args[1]})>
    when 'comment'
      nil
    else
      ''
    end
  end

  def is_jpg(uri)
    if /\.jpg$/ =~ uri then
      return true
    end
    if /\.jpeg$/ =~ uri then
      return true
    end
    if /\.png$/ =~ uri then
      return true
    end
    return false
  end

  def a_href(uri, label, cssclass)
    str = label.strip
    if(cssclass.casecmp('pagelink')==0) then
      if(uri.size===0) then
#        %Q<[#{str}](#{@base_uri}#{escape(str)}.html)>
        %Q<[#{str}](#{escape(str)}.md) >
      else
#        %Q<[#{str}](#{@base_uri}#{escape(uri.strip)}.html)>
        if /^http/ =~ uri.strip then
          %Q<[#{str}](#{escape(uri.strip)}) >
		else
          %Q<[#{str}](#{escape(uri.strip)}.md) >
		end
      end
    else
      #%Q<[#{str}](#{URI.escape(uri.strip)})>
	  if(is_jpg(uri.strip)) then
        %Q<![#{str}](#{uri_encode(uri.strip)}) >
      else
         %Q<[#{str}](#{uri_encode(uri.strip)}) >
      end
    end
  end

  def autolink_re
    Regexp.union(* @page_names.reject {|name| name.size <= 3 })
  end

  def page_uri(page_name)
    "#{@base_uri}#{urldecode(page_name)}#{@pagelist_suffix}"
  end
end

def main
  include HTMLUtils
  srcpath = ARGV[0]
  tgtpath = ARGV[1]
  srcmask = ARGV[2] ? ARGV[2] : "*.txt"

  if File.exist?(srcpath)
    Dir::glob("#{srcpath}/#{srcmask}").each {|f|
      fname = File.basename(f)
      tbody = File.read(f)
      page_names = []
      parser = PukiWikiParser.new()
      buf    = parser.to_md(tbody, page_names, HTMLUtils.urldecode(fname))
      tmp    = parser.filename(fname)

      puts fname

#      if parser.has_pubdate then
        nname  = [tgtpath, tmp].join('/')
        puts tmp
        outf = open(nname, "wb")
        outf.puts(buf)
        outf.close()
#      end
    }
  else
    puts srcpath
    puts "No such directory"
  end
end
main
