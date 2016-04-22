#!/usr/local/bin/perl

# use strict; use warnings;

$::PROTOCOL = '(?:s?https?|ftp)';
$::RXURL    = '(?:s?https?|ftp)://[-\\w.!~*\'();/?:@&=+$,%#]+' ;
$::charset  = 'EUC-JP';
$::version  = '1.0.2.4';
%::form     = ();
$::me       = $::postme = ( split(/[\/\\]/,$0) )[-1];
$::print    = ' 'x 10000; $::print = '';
%::config   = ( crypt => 'tekitou' , sitename => 'wifky!' );
$::psuffix   = '.pl';
$::syntax_engine = \&default_syntax_engine;

binmode(STDOUT);
binmode(STDIN);

eval{
    &read_form(\%::form);
    &change_directory;
    foreach my $pl (sort grep(/\.plg$/,&directory) ){
        do $pl; $@ and die($@);
    }
    &load_config;
    &init_globals;
    foreach my $pl (sort grep(/$::psuffix$/o,&directory) ){
        do $pl; $@ and die($@);
    }

    if( exists $::form{a} && exists $::action_plugin{$::form{a}} ){
        $::action_plugin{ $::form{a} }->();
    }elsif( exists $::form{p} ){ # page view
        if( exists $::form{f} ){ # output attachment
            &action_cat;
        }else{ # output page itself.
            &action_view($::form{p});
        }
    }elsif( &object_exists('FrontPage') ){ # default(no-option)
        &action_view('FrontPage');
    }else{
        &do_index('recent','rindex','-l');
    }
    &flush;
};
if( $@ ){
    print "Content-Type: text/html;\n\n<html><body>\n",
          &errmsg($@),"\n</body></html>\n";
}
exit(0);

sub change_directory{
    my $pagedir = $0 ; $pagedir =~ s/\.\w+$/.dat/;
    unless( chdir $pagedir ){
        mkdir($pagedir,0755);
        chdir $pagedir or die("can not access $pagedir.");
    }
}

sub init_globals{
    if( &is('locallink') ){
        $::PROTOCOL = '(?:s?https?|ftp|file)';
        $::RXURL    = '(?:s?https?|ftp|file)://[-\\w.!~*\'();/?:@&=+$,%#]+';
    }

    $::target = ( $::config{target}
                ? sprintf(' target="%s"',$::config{target}) : '' );
    $::config{CSS} or $::config{CSS} = 'CSS';
    $::config{cssstyle} or $::config{cssstyle} = 'OK';
    %::inline_plugin = (
        'adminmenu'=> \&plugin_menubar ,
        'menubar'  => \&plugin_menubar ,
        'pagename' => \&plugin_pagename ,
        'recent'   =>
            sub{ '<ul>'.&ls('-r','-t',map("-$_",@_[1..$#_])) . '</ul>' } ,
        'search'   => \&plugin_search ,
        'fn'       => \&plugin_footnote ,
        'ls'       => sub{ '<ul>' . &ls(map(&denc($_),@_[1..$#_])) . '</ul>' },
        'comment'  => \&plugin_comment ,
        'sitename' => sub{ &enc(exists $::config{sitename} ?
                                $::config{sitename} : '') } ,
        'br'       => sub{ '<br>' } ,
        'clear'    => sub{ '<br clear="all">' } ,
        'lt'       => sub{ '&lt;' } ,
        'gt'       => sub{ '&gt;' } ,
        'amp'      => sub{ '&amp;' } ,
        'lp'       => sub{ '&#40;' } ,
        'rp'       => sub{ '&#41;' } ,
        'lb'       => sub{ '&#91;' } ,
        'rb'       => sub{ '&#93;' } ,
        'll'       => sub{ '&#40;&#40;' },
        'rr'       => sub{ '&#41;&#41;' },
        'vl'       => sub{ '&#124;' },
        'v'        => sub{ '&' . ($#_ >= 1 ? $_[1] : 'amp') . ';' },
        'bq'       => sub{ '&#96;' },
        'null'     => sub{ '' } ,
    );

    %::action_plugin = (
        'index'         => sub{ &do_index('recent','rindex','-l');  },
        'rindex'        => sub{ &do_index('recent','index' ,'-l','-r'); },
        'older'         => sub{ &do_index('recent','index' ,'-l','-t'); },
        'recent'        => sub{ &do_index('older' ,'index' ,'-l','-t','-r');},
        '?'             => \&action_seek ,
        'edt'           => \&action_edit ,
        'pwd'           => \&action_passwd ,
        'ren'           => \&action_rename ,
        'del'           => \&action_delete ,
        'comment'       => \&action_comment ,
        'Delete'        => \&action_query_delete ,
        'Commit'        => \&action_commit ,
        'Preview'       => \&action_preview ,
        'Upload'        => \&action_upload ,
        'tools'         => \&action_tools ,
        'preferences'   => \&action_preferences ,
        'new'           => \&action_new ,
    );

    @::http_header = ( "Content-type: text/html; charset=$::charset" );

    @::html_header = (
      qq(<meta http-equiv="Content-Type"
        content="text/html; charset=$::charset">
        <meta http-equiv="Content-Style-Type" content="text/css">
        <meta name="generator" content="wifky.pl $::version">
        <link rel="start" href="$::me">
        <link rel="index" href="$::me?a=index">)
    );

    @::body_header = <DATA>;

    @::menubar = (
        qq(<a href="$::me">FrontPage</a>) ,
        qq(<a href="$::me?a=new" rel="nofollow">New</a>) ,
    );

    ### menubar ###
    unless( exists $::form{a} ){
        my $url=&title2url(exists $::form{p} ? $::form{p} : 'FrontPage');
        &is_frozen() or
            push(@::menubar,qq(<a href="$url;a=edt" rel="nofollow">Edit</a>) );
        push( @::menubar ,
            qq{<a href="$url;a=edt;admin" rel="nofollow">Edit(Admin)</a>} );
    }
    push(@::menubar ,
            qq(<a href="$::me?a=tools" rel="nofollow">Tools</a>) ,
            qq(<a href="$::me?a=recent">Index</a>) );

    @::copyright = (
        qq(Generated by <a href="http://hpcgi2.nifty.com/hayamatta/wifky.pl"
                >wifky</a> $::version with Perl $])
    );

    @::formtext = ();

    %::preferences = (
        ' General Options' => [
            { desc=>'the sitename', name=>'sitename', size=>40 },
            { desc=>'enable link to file://...', name=>'locallink' ,
              type=>'checkbox', size=>3 },
            { desc=>'forbid any one but administrator creating a new page.' ,
              name=>'lonely' , type=>'checkbox' },
            { desc=>'imitate tDiary\'s style (put header in mainframe..)' ,
              name=>'cssstyle' , type=>'checkbox' },
            { desc=>'target value for external link.',name=>'target'},
            { desc=>'pagename or url for CSS' , name=>'CSS' , size=>40 },
        ],
        ' Section Marks' => [
            { desc=>'section mark', name=>'sectionmark', size=>3 } ,
            { desc=>'subsection mark' , name=>'subsectionmark' , size=>3 } ,
            { desc=>'subsubsection mark' , name=>'subsubsectionmark' , size=>3 }
        ]
    );
    if( &is('cssstyle') ){
        %::ss = (
            terminator => 'terminator' ,
            copyright  => 'copyright footer' ,
            begin_comment => '<div>', end_comment => '</div>',
            begin_comment1 => '<p>' , end_comment1 => '</p>',
        );
    }else{
        %::ss = (
            terminator => 'terminator footer' ,
            copyright  => 'copyright' ,
            begin_comment => '<ul class="comment">', end_comment => '</ul>',
            begin_comment1 => '<li>' , end_comment1 => '</li>',
        );
    }
}

sub read_multimedia{
    my ($query_string , $cutter , $form ) = @_;

    my @blocks = split("\r\n${cutter}","\r\n$query_string");
    foreach my $block (@blocks){
        $block =~ s/\A\r?\n//;

        my ($header,$body) = split(/\r?\n\r?\n/,$block,2);
        my ($type,$name,$fname) = ( '' , 'notitle' , 'attach.bin' );

        if( $header =~ /^Content-Disposition:\s+form-data;\s+name=\"(\w+)\"/i){
            $name = $1;
            $header =~ /filename="([^\"]+)"/
                and $fname=(split(/[\/\\]/,$1))[-1];
        }
        ($header =~ /^Content-Type: image\/(\w+)/ ) and $type = $1;

        $form->{ $name } = $body;
        length($type) > 0 and $form->{ "${name}.type" } = $type;
        $form->{ "${name}.filename" } = $fname;
    }
}

sub read_form{
    my $form=shift;
    my $query_string;
    if( exists $ENV{REQUEST_METHOD} && $ENV{REQUEST_METHOD} eq 'POST' ){
        $ENV{CONTENT_LENGTH} > 1024*1024 and die('Too large form data');
        read(STDIN, $query_string, $ENV{CONTENT_LENGTH});
        if( $query_string =~ /\A(-------.*?)\r?\n/ ){
            &read_multimedia($query_string,$1,$form);
            return;
        }
    } else {
        $query_string = (exists $ENV{QUERY_STRING} ? $ENV{QUERY_STRING} : '');
    }
    foreach my $p ( split(/[&;]/, $query_string) ){
        my ($name, $value) = split(/=/, $p,2);
        defined($value) or $value = '' ;
        $value =~ s/\+/ /g;
        $value =~ s/%([0-9a-fA-F][0-9a-fA-F])/pack('C', hex($1))/eg;
        $form->{$name} = $value;
    }
}

sub puts{
    grep(($::print .= "$_\r\n",0),@_);
}

# puts with auto escaping arguments but format-string.
sub putenc{
    my $fmt=shift;
    $::print .= sprintf("$fmt\r\n",map(&enc($_),@_));
}

sub flush{
    $::print =~ s|\a((?:[0-9a-f][0-9a-f])*)\a|pack('h*',$1)|ges;
    defined( $::hook_flush ) and $::hook_flush->( \$::print );
    print $::print;
}

sub errmsg{
    '<h1>Error !</h1><pre>'
    . &enc( $_[0] =~ /^\!([^\!]+)\!/ ? $1 : $_[0] )
    . '</pre>';
}

sub enc{
    my $s=shift;
    defined($s) or return '';
    $s =~ s/&/\&amp;/g;
    $s =~ s/</\&lt;/g;
    $s =~ s/>/\&gt;/g;
    $s =~ s/"/\&quot;/g;
    $s =~ s/'/\&#39;/g;
    $s =~ tr/\r\a\b//d;
    $s;
}
sub denc{
    my $s = shift;
    defined($s) or return '';
    $s =~ s/\&#39;/'/g;
    $s =~ s/\&lt;/</g;
    $s =~ s/\&gt;/>/g;
    $s =~ s/\&quot;/\"/g;
    $s =~ s/\&amp;/\&/g;
    $s;
}

sub yen{ # to save crlf-code into hidden.
    my $s = shift;
    $s =~ s/\^/\^y/g;
    $s =~ s/\r/\^r/g;
    $s =~ s/\n/\^n/g;
    $s =~ s/\t/\^t/g;
    $s ;
}

sub deyen{
    my $s = shift;
    $s =~ s/\^t/\t/g;
    $s =~ s/\^n/\n/g;
    $s =~ s/\^r/\r/g;
    $s =~ s/\^y/\^/g;
    $s ;
}

sub mtime{
    unless( exists $::mtime_cache{ $_[0] } ){
        my @tm=localtime((stat($_[0]))[9]);
        $::mtime_cache{ $_[0] } = sprintf('%04d/%02d/%02d %02d:%02d:%02d'
            , 1900+$tm[5],1+$tm[4],@tm[3,2,1,0]);
    }
    $::mtime_cache{ $_[0] };
}

sub cacheoff{
    undef %::mtime_cache;
    undef @::work_directory_cache;
    undef %::work_directory_cache;
}
sub title2mtime{
    &mtime( &title2fname(@_) );
}
sub fname2title{
    pack('h*',$_[0]);
}
sub title2fname{
    join('__',map(unpack('h*',$_),@_) );
}
sub percent{
    my $e_title = shift;
    $e_title =~ s/([^\w\'\.\-\*\(\)\_ ])/sprintf('%%%02X',ord($1))/eg;
    $e_title =~ s/ /+/g;
    $e_title;
}

sub title2url{
    $::me . '?p=' . &percent( $_[0] );
}

sub attach2url{
    $::me . '?p=' . &percent($_[0]) . ';f=' . &percent($_[1]);
}

sub is{ exists $::config{$_[0]} && $::config{$_[0]} eq 'OK' ; }

sub print_form{
    my ($title,$html,$stamp) = @_;

    &putenc('<div class="update"
        ><form action="%s" enctype="multipart/form-data" method="post"
        ><input type="hidden" name="stamp" value="%s"
        ><input type="hidden" name="p" value="%s"><br>'
        , $::postme,$stamp,$title );
    &puts('<textarea cols="60" rows="15" name="honbun">'.$html.
            '</textarea><br>');

    my $fname=&title2fname( $::form{p} );
    &puts('<input type="submit" name="a" value="Preview">');
    if( exists $::form{admin} || &is_frozen() ){
        &puts('Administrator\'s Sign:
        <input type="password" name="password"
        ><input type="checkbox" name="to_freeze" value="1"');
        &is_frozen() and &puts('checked');
        &puts(' >freeze <input type="hidden" name="admin" value="admin">');
    }
    &puts('<input type="submit" name="a" value="Commit">');
    if( &object_exists($::form{p}) ){
        &puts('<h2>Attachment</h2
        ><p>New:<input type="file" name="butsu" size="48">');
        &is_frozen() and &puts('Sign:<input type="password" name="qassword">');
        &puts('<input type="submit" name="a" value="Upload"></p>');

        my @attachments=&list_attachment( $::form{p} );
        if( scalar(@attachments) > 0 ){
            &puts('<p>');
            foreach my $attach (sort @attachments){
                &putenc('<input type="radio" name="f" value="%s" ><input
                        type="text" name="dummy" readonly
                        value="&lt;&lt;{%s}" size="20"
                        style="font-family:monospace">'
                          ,$attach ,$attach );

                my $fn = &title2fname($::form{p}, $attach);
                &putenc('(%d bytes, at %s)',(stat $fn)[7],&mtime($fn));
                &puts('<br>');
            }
            &puts('</p><input type="submit" name="a" value="Delete">
                <input type="submit" name="dummybotton" value="Download">');
        }
    }
    &puts(@::formtext);
    &puts('</form></div>');
}

sub print_menubar_once{
    $::menubar_printed or &puts( &plugin({},'menubar') );
}

sub default_header{ ### No page named 'Header'
    &puts('<div class="header">');
    $_[0] and &print_menubar_once();
    if( &is('cssstyle') ){
        &putenc('<h1>%s</h1>',$::config{sitename});
    }else{
        &putenc('<div class="sitename">%s</div>',$::config{sitename});
        &puts('<h1><span class="pagename">'
                , &plugin_pagename() , '</span></h1>');
    }
    &puts('</div>');
}

sub print_header{
    my %arg=(@_);
    my $label = $::config{sitename};
    exists $::form{p}  and $label .= ' - ' . $::form{p};
    exists $arg{title} and $label .= '(' . $arg{title} . ')';
    my $require_menubar = !(exists $arg{menubar} && $arg{menubar} eq 'no');
    my $divclass = ($arg{divclass} || 'main');

    &puts( @::http_header , '' );
    &putenc('<?xml version="1.0" encoding="%s"?>
        <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
        <html lang="ja"><head>',$::charset);
    &puts( @::html_header );
    &putenc('<title>%s</title>',$label);
    my $cssn=$::config{CSS};
    if( $cssn =~ /^http:/ || $cssn =~ /^\./ ){ ### URL for CSS
        &putenc('<link rel="stylesheet" type="text/css" href="%s">',$cssn);
    }else{ ### PAGE for CSS
        my $css =&read_object($cssn);
        if( $css ){
            $css =~ s/[<>&]//g;
            &puts(qq(<style type="text/css"><!--\n${css}\n--></style>));
        }
    }
    &puts('</head>');
    &puts( exists $::form{p} && &is_frozen($::form{p})
                ? '<body class="frozen">' : '<body>' );
    &puts( @::body_header );
    &is('cssstyle') and &putenc('<div class="%s">' , $divclass );

    $arg{userheader}
        and !&print_page( title=>'Header', class=>'header')
        and &default_header( $require_menubar );

    &is('cssstyle') or &putenc('<div class="%s">' , $divclass );

    $require_menubar and &print_menubar_once();
}

sub print_footer{
    &puts('</div><!--main or sidebar-->');
    &puts('</body></html>');
}

sub print_sidebar_and_footer{
    &puts('</div><!-- main --><div class="sidebar">');
    &print_page( title=>'Sidebar' );
    &print_footer;
}

sub print_copyright{
    &puts('<div class="'.$::ss{copyright}.'">',@::copyright,'</div>');
}

sub is_frozen{
# 2014-10
	return 1; 
#
    if( -r &title2fname(exists $::form{p} ? $::form{p} : 'FrontPage' ) ){
        ! -w _;
    }else{
        &is('lonely');
    }
}

sub ninsho{
    if( $::config{crypt} ne '' &&
        crypt($::form{password},'wk') ne $::config{crypt} &&
        crypt($::form{qassword},'wk') ne $::config{crypt} )
    {
        die('!Administrator\'s Sign is wrong!');
    }
}

sub check_frozen{
    if( exists $::form{admin} ){ ### Administrator mode ###
         &ninsho;
    }elsif( &is_frozen() ){ ### User ###
        die( '!This page is frozen.!');
    }
}
sub check_conflict{
    my $stamp_time = &title2mtime($::form{p});
    my $begin_time = $::form{stamp};
    if( $begin_time ne $stamp_time ){
        die( "!Someone else modified this page on ${stamp_time}".
             " after you began to edit on ${begin_time}!"  );
    }
}

sub read_object{
    return &read_file(&title2fname(@_));
}

sub read_file{
    open(FP,$_[0]) or return '';
    local $/; undef $/;
    my $object = <FP>;
    close(FP);
    defined($object) ? $object : '';
}

# write object with OBJECT-NAME(S) , not filename.
sub write_object{
    my $body  = pop(@_);
    my $fname = &title2fname(@_);
    &write_file($fname,$body);
}

sub write_file{
    my ($fname,$body) = @_;

    if( length( ref($body) ? $$body : $body ) <= 0 &&
        scalar( grep(index($_,"${fname}__")==0 , &directory())) == 0 )
    {
        unlink $fname;
        &cacheoff;
        0;
    }else{
        open(FP,">${fname}") or die("can't write the file ${fname}.");
            binmode(FP);
            print FP ref($body) ? ${$body} : $body;
        close(FP);
        &cacheoff;
        1;
    }
}

sub action_new{
    &print_header( divclass=>'max' );
    &puts(qq(<h1>Create Page</h1>
        <form action="$::postme" method="post">
        <p><input type="text" name="p" size="40">
        <input type="hidden" name="a" value="edt">
        <input type="submit" value="Create"></p></form>));
    &print_footer;
}

sub load_config{
    defined($::load_config) and return &$::load_config;
    rename(&title2fname('','password'),'index.cgi');
    grep( (/^\#?([^\#\!\t ]+)\t(.*)$/ and $::config{$1}=&deyen($2),0)
        , split(/\n/,&read_file('index.cgi') ) );
}

sub save_config{
    defined($::save_config) and return &$::save_config;
    my @settings;
    while( my ($key,$val)=each %::config ){
        push( @settings , "#$key\t$val" );
    }
    &write_file( 'index.cgi' , join("\n", @settings) );
}

sub action_query_delete{
    &print_header( divclass=>'max' );
    &puts(qq(<form action="$::postme" method="post">));
    &putenc( q( <h1>Remove attachment</h1><p>
                Remove attachment '%s' of '%s'.<br>)
            , $::form{f} , $::form{p} );
    &is_frozen() and &puts( q(Administrator's Sign:
            <input type="password" name="password"><br>) );
    &putenc('Are you sure ? <br>
        <input type="submit" name="yes" value="Yes">
        <input type="submit" name="no" value="No">
        <input type="hidden" name="a" value="del">
        <input type="hidden" name="p" value="%s">
        <input type="hidden" name="f" value="%s">
        <input type="hidden" name="stamp" value="%s">
        <input type="hidden" name="encrlf" value="YES">'
            , $::form{p} , $::form{f} , $::form{stamp} );
    &putenc( '<input type="hidden" name="honbun" value="%s"></p></form>',
                &yen($::form{honbun}) );

    &print_footer;
}

sub action_commit{
    my $lock=&title2fname($::form{p},'LOCK');
    eval{
        mkdir $lock,0777 or die('!File writing conflict!');
        &check_frozen;
        &check_conflict;
    };
    if( $@ ){
        &do_preview( &errmsg($@) );
    }else{
        &do_submit;
    }
    rmdir $lock;
}

sub action_preview{
    eval{
        &check_conflict;
    };
    if( $@ ){
        &do_preview( &errmsg($@) );
    }else{
        &do_preview;
    }
}

sub action_passwd{
    my ($p1,$p2) = ( $::form{p1} , $::form{p2} );
    &ninsho;
    ( $p1 ne $p2 ) and die("!New signs differ from each other!");
    $::config{crypt} = crypt($p1,"wk");
    &save_config;
    &transfer_url($::me);
}

sub action_tools{
    &print_header( divclass=>'max' , title=>'Tools' );
    &putenc('<h1>Tools</h1><div class="day">
        <h2>Change Administrator\'s Sign</h2
        ><div class="body"
        ><form action="%s" method="post"
        ><p>Old Sign:<input name="password" type="password" size="40"
        ><br>New Sign(1):<input name="p1" type="password" size="40"
        ><br>New Sign(2):<input name="p2" type="password" size="40"
        ><br><input name="a" type="hidden"  value="pwd"
        ><input type="submit" value="Submit"></p></form
        ></div></div><div class="day"
        ><h2>Preferences</h2><div class="body"
        ><form action="%s" method="post">',$::postme,$::postme);

    foreach my $section(sort keys %::preferences){
        &putenc('<div class="section"><h3>%s</h3><div class="sectionbody"><p>'
                    ,$section);
        foreach my $i ( @{$::preferences{$section}} ){
            if( $i->{type} eq 'checkbox' ){
                &putenc('<input type="checkbox" name="%s" value="OK"%s> %s<br>'
                    , $i->{name}
                    , ( &is($i->{name}) ? 'checked' : '' )
                    , $i->{desc}
                );
            }elsif( $i->{type} eq 'password' ){
                &putenc('%s <input type="password" name="%s">
                        (retype)<input type="password" name="%s_"><br>'
                    , $i->{desc} , $i->{name} , $i->{name}
                );
            }else{ # text
                &putenc(
                    '%s <input type="text" name="%s" value="%s" size="%s"><br>'
                    , $i->{desc} , $i->{name}
                    , exists $::config{$i->{name}} ? $::config{$i->{name}} : ''
                    , $i->{size} || 10
                );
            }
        }
        &puts('</p></div></div>');
    }
    &puts('Administrator\'s Sign:
        <input type="password" name="password"
        ><input type="hidden" name="a" value="preferences"
        ><input type="submit" value="Submit"></form></div></div>');

    &print_footer;
}

sub action_preferences{
    &ninsho;
    foreach my $section ( %::preferences ){
        foreach my $i (@{$::preferences{$section}} ){
            if( $i->{type} eq 'checkbox' ){
                $::config{ $i->{name} } =
                    ( $::form{ $i->{name} } eq 'OK' ? 'OK' : 'NG' );
            }elsif( $i->{type} eq 'password' ){
                if( length($::form{$i->{name}}) > 0 ){
                    $::form{$i->{name}} eq $::form{$i->{name}.'_'}
                        or die('invalud value for ' . $i->{name} );
                    $::config{ $i->{name} } = $::form{$i->{name} };
                }
            }else{
                $::config{ $i->{name} } = $::form{ $i->{name} };
            }
        }
    }
    &save_config;
    &transfer_url($::me);
}

sub action_rename{
    &ninsho;
    my $newtitle = $::form{newtitle};
    my $title    = $::form{p};
    my $fname    = &title2fname($title);
    my $newfname = &title2fname($newtitle);

    foreach my $fn ( &directory() ){
        my @p=split('__',$fn);
        if( $p[0] eq $fname ){
            $p[0] = $newfname;
            rename( $fn , join('__',@p) );
        }
    }
    &transfer_page($newtitle);
}

sub action_seek{
    my $keyword=$::form{keyword};
    my $ekeyword=&enc( $keyword );

    &print_header( title=>qq(Seek: "$ekeyword") , userheader=>1 );
    &begin_day(qq(Seek: "$ekeyword"));
    &puts('<ul>');
    foreach my $fn ( &list_page() ){
        my $title  = &fname2title( $fn );
        my $url    = &title2url( $title );
        if( index($title ,$keyword) >= 0 ){
            &putenc('<li><a href="%s">%s</a> (title)</li>'
                    , $url , $title );
        }else{
            open(FP,$fn) or die("Can not open the file $fn");
            while( <FP> ){
                if( index($_,$keyword) >= 0 ){
                    &putenc('<li><a href="%s">%s</a></li>' , $url , $title );
                    last;
                }
            }
            close(FP);
        }
    }
    &puts('</ul>');
    &end_day();
    &print_copyright;
    &print_sidebar_and_footer;
}

sub action_delete{
    if( exists $::form{yes} ){
        &is_frozen() and &ninsho;
        unlink( &title2fname( $::form{p} , $::form{f} ) );
        &cacheoff;
    }
    &do_preview;
}

sub action_comment{
    my $title   = $::form{p};
    my $comid   = $::form{comid};
    my $who     = $::form{who} ;
    my $comment = $::form{comment};

    if( length($comment) > 0 ){
        utime( time , time , &title2fname($title) ) <= 0
            and die("unable to comment to unexistant page.");
        &cacheoff;
        my $fname  = &title2fname($title,"comment.${comid}");
        open(FP,">>${fname}") or die("Can not open $fname for append");
            my @tm=localtime;
            printf FP "%04d/%02d/%02d %02d:%02d:%02d\t%s\t%s\r\n"
                , 1900+$tm[5],1+$tm[4],@tm[3,2,1,0]
                , &yen($who) , &yen($comment) ;
        close(FP);
    }
    my $ecomid = &enc($comid);
    &transfer_page;
}

sub begin_day{
    &puts('<div class="day">');
    &is('cssstyle') and &putenc('<h2><span class="title">%s</span></h2>',$_[0]);
    &puts('<div class="body">');
}

sub end_day{ &puts('</div></div>'); }

sub do_index{
    my $t=shift;
    my $n=shift;

    &print_header( title=>'IndexPage' , userheader=>1 );
    &begin_day('IndexPage');
        &puts(qq(<ul><li><tt><a href="$::me?a=${t}"> Last Modified Time</a
            >&nbsp;<a href="$::me?a=${n}">Page Title</a></tt></li>));
        &puts( &ls(@_) ,'</ul>');
    &end_day();
    &print_copyright;
    &print_sidebar_and_footer;
}

sub action_upload{
    exists $::form{p} or die('not found pagename');
    &check_frozen;
    &write_object( $::form{p} , $::form{'butsu.filename'} , \$::form{butsu});
    &do_preview;
}

sub do_submit{
    my $title=$::form{p};
    my $fn=&title2fname($title);

    &is_frozen() and chmod(0644,$fn);

    defined($::hook_submit) and $::hook_submit->(\$title , \$::form{honbun});
    if( &write_object( $title , \$::form{honbun} ) ){
        ( $::form{to_freeze} eq '1' ) and chmod 0444,$fn;
        &transfer_page;
    }else{
        &transfer_url($::me);
    }
}

sub transfer_url{
    my $url=(shift || $::me);
    &puts('Content-type: text/html');
    &puts('');
    &puts(qq(<html><head><title>Moving...</title
        ><meta http-equiv="refresh" content="1;URL=${url}"
        ></head><body
        ><a href="${url}">Wait or Click Here</a
        ></body></html>));
}

sub transfer_page{
    &transfer_url( &title2url($#_ >= 0 ? $_[0] : $::form{p} ) );
}

sub do_preview{
    my $e_message  = shift;
    my $title = $::form{p};
    my $html  = &enc($::form{honbun});
    exists $::form{encrlf} && $::form{encrlf} eq 'YES' and $html=&deyen($html);

    &print_header(divclass=>'max',title=>'Edit');
    defined($e_message) and &puts(qq(<div class="warning">${e_message}</div>));
    &begin_day("Preview: $title");
        &print_page( title=>$title , html=>$html , index=>1 , main=>1 );
    &end_day();
    &print_form( $title , $html , $::form{stamp} );
    &print_footer;
}

sub action_edit{
    my $title = $::form{p};
    &print_header(divclass=>'max',title=>'Edit');
    &putenc('<h1>Edit: %s</h1>',$title);
    &print_form( $title , &enc(&read_object( $title )), &title2mtime($title) );

    if( &object_exists($::form{p}) && exists $::form{admin} ){
        &putenc('<h2>Rename</h2>
            <p>
            <form action="%s" method="post">
            <input type="hidden"  name="a" value="ren">
            <input type="hidden"  name="p" value="%s">
            Title: <input type="text" name="newtitle" value="%s" size="80">
            <br>Administrator\'s Sign:
            <input type="password" name="password">
            <br><input type="submit" name="ren" value="Submit">
            </form>
            </p>
        ' , $::postme , $::form{p} , $::form{p} );
    }
    &print_footer;
}

sub action_view{
    my $title = $::form{p} = shift;
    &print_header( userheader=>1 );
    &begin_day( $title );
        &print_page( title=>$::form{p} , index=>1 , main=>1 );
    &end_day();
    &print_page( title=>'Footer' , class=>$::ss{terminator} );
    &print_copyright;
    &print_sidebar_and_footer;
}

sub action_cat{
    my $attach=$::form{f};
    my $path=&title2fname($::form{p},$attach);

    open(FP,$path) or die('Can not found the filename');
    binmode(FP);
    binmode(STDOUT);

    my $type= $attach =~ /\.gif$/i ? 'image/gif'
            : $attach =~ /\.jpg$/i ? 'image/jpeg'
            : $attach =~ /\.png$/i ? 'image/png'
            : $attach =~ /\.pdf$/i ? 'application/pdf'
            : $attach =~ /\.txt$/i ? 'text/plain'
            : 'application/octet-stream';

    print  qq(Content-Disposition: attachment; filename="${attach}"\r\n);
    print  qq(Content-Type: ${type}\r\n);
    printf qq(Content-Length: %d\r\n),( stat(FP) )[7];
    printf qq(Last-Modified: %s, %02d %s %04d %s GMT\r\n) ,
                (split(' ',scalar(gmtime((stat(FP))[9]))))[0,2,1,4,3];
    print  qq(\r\n);
    print <FP>;
    close(FP);
}

sub directory{
    unless( defined(@::work_directory_cache) ){
        opendir(DIR,'.') or die('can\'t read work directory.');
        grep( ($::work_directory_cache{ $_ } = 1,0)
            ,  @::work_directory_cache = readdir(DIR) );
        closedir(DIR);
    }
    @::work_directory_cache;
}

sub list_page{
    grep( /^([0-9a-z][0-9a-z])+$/ , &directory );
}

sub object_exists{
    exists $::work_directory_cache{ &title2fname($_[0]) };
}

sub list_attachment{
    my $prefix = &title2fname( shift ) . '__';
    my $off = length($prefix);
    map(&fname2title(substr($_,$off)),grep(index($_,$prefix)==0,&directory));
}

sub print_page{
    my %args=( @_ );
    my $title=$args{title};
    my $html =( exists $args{html} ? $args{html} : &enc( &read_object($title)) );
    return 0 unless $html;

    my %attachment;
    foreach my $attach ( &list_attachment($title) ){
        my $e_attach = &enc( $attach );
        my $url=&attach2url($title,$attach);
        $attachment{ $e_attach } = {
            name => $attach ,
            url  => $url ,
            tag  => $attach =~ /\.(png|gif|jpg|jpeg)$/i
                    ? qq(<img src="${url}" alt="${e_attach}">)
                    : qq(<a href="${url}" title="${e_attach}">${e_attach}</a>) ,
        };
    }
    my %session=(
        title      => $title ,
        attachment => \%attachment ,
        index      => $args{index} ,
        main        => $args{main} ,
    );
    if( exists $args{class} ){
        &puts(qq(<div class="$args{class}">));
        &syntax_engine( \$html , \%session );
        &puts('</div>');
    }else{
        &syntax_engine( \$html , \%session );
    }
    1;
}

sub verb{
    "\a".unpack('h*',$_[0])."\a";
}

sub verbatim{
    my $html=shift;
    $$html =~ s!^\s*\&lt;pre&gt;(.*?\n)\s*\&lt;/pre&gt;|^\s*8\&lt;(.*?\n)\s*\&gt;8|`(.)`(.*?)`\3`!
                  defined($4)
                ? &verb('<tt class="pre">'.&cr2br($4).'</tt>')
                : "\n\n<pre>".&verb(defined($1) ? $1 : $2)."</pre>\n\n"
            !gesm;
}

sub inner_link{
    my $symbol = shift;
    my $title  = &denc( shift );

    if( &object_exists($title) ){
        my $url=&title2url($title);
        &verb(qq(<a href="${url}">)).$symbol.'</a>';
    }else{
        qq(<blink>${symbol}?</blink>);
    }
}

sub plugin_menubar{
    shift;
    $::menubar_printed=1;
    '<p class="adminmenu menubar">'. join("\r\n",
        map('<span class="adminmenu">'.$_.'</span>',@_,@::menubar)).'</p>';
}

sub plugin_search{
    sprintf( '<div class="search_form"><form class="search" action="%s">
        <input class="search" type="text" name="keyword" size="20" value="%s">
        <input class="search" type="submit" name="a" value="?">
        </form></div>' ,
        $::me ,
        &enc(exists $::form{keyword} ? $::form{keyword} : '' ));
}

sub plugin_footnote{
    my $session = shift;
    my $footnotetext=$session->{argv};
    my $footnotes = ($session->{footnotes} || ($session->{footnotes}=[]));
    push(@{$footnotes} , $footnotetext );

    $footnotetext =~ s|\a((?:[0-9a-f][0-9a-f])*)\a|pack('h*',$1)|ges;
    $footnotetext =~ s/\r?\n/ /g;
    $footnotetext =~ s/\<[^\>]*\>//g;

    my $i=$#{$footnotes} + 1;
    qq(<sup><a href="#ft${i}" name="fm${i}" title="${footnotetext}">*${i}</a></sup>);
}

sub plugin_footnote_flush{
    my $session = shift;
    my $footnotes = $session->{footnotes};
    return unless $footnotes;

    my $i=0;
    &puts(qq(<div class="footnote">));
    foreach my $t (@{$footnotes}){
        ++$i;
        &puts(qq(<p class="footnote"><a name="ft${i}" href="#fm${i}"
                >*${i}</a> $t</p>));
    }
    &puts(qq(</div><!--footnote-->));
    delete $session->{footnotes};
}

sub ls_core{
    my $opt = shift;
    my @list;
    push(@_,'*') unless @_;

    foreach (@_){
        my $pat=$_;
        $pat =~ s/([^\*\?]+)/unpack('h*',$1)/eg;
        $pat =~ s/\?/../g;
        $pat =~ s/\*/.*/g;
        $pat = '^' . $pat . '$';
        push(@list, map({ fname  => $_ ,
                          title  => &fname2title($_) ,
                          mtime  => &mtime($_)
                        } , grep($_ =~ $pat, &list_page() ) ) );
    }
    if( exists $opt->{t} ){
        @list = sort{ $a->{mtime} cmp $b->{mtime} } @list;
    }else{
        @list = sort{ $a->{title} cmp $b->{title} } @list;
    }
    exists $opt->{r}         and @list = reverse @list;
    exists $opt->{number}    and splice(@list,$opt->{number});
    exists $opt->{countdown} and splice(@list,$opt->{countdown});
    @list;
}

sub parse_opt{
    my $opt=shift;
    my $arg=shift;
    foreach my $p (@_){
        if( $p =~ /^-(\d+)$/ ){
            $opt->{number} = $opt->{countdown} = $1;
        }elsif( $p =~ /^-/ ){
            $opt->{$'} = 1;
        }else{
            push(@{$arg},$p);
        }
    }
}

sub ls{
    my $buf = '';
    my %opt=();
    my @arg=();
    &parse_opt(\%opt,\@arg,@_);

    foreach my $p ( &ls_core(\%opt,@arg) ){
        $buf .= '<li>';
        exists $opt{l} and $buf .= '<tt>'.$p->{mtime}.' </tt>';
        $buf .= sprintf(qq(<a href="%s">%s</a></li>\r\n)
                    , &title2url($p->{title})
                    , &enc($p->{title}) );
    }
    $buf;
}

sub plugin_comment{
    my $session=shift;
    my @arg; my %opt;
    &parse_opt( \%opt , \@arg , @_ );
    my $etitle= &enc($::form{p});
    my $comid = ($arg[0] || '0');
    my $caption = $#arg >= 1
        ? '<div class="caption">'.join(' ',@arg[1..$#arg]).'</div>'
        : '';

    exists $session->{"comment.$comid"} and return '';
    $session->{"comment.$comid"} = 1;

    my $ecomid = &enc($comid);
    my $fname=&title2fname( $::form{p} , "comment.$comid" );
    my $buf = '<div class="comment">'.$caption.'<div class="commentshort">';
    if( open(FP, $fname) ){
        $buf .= $::ss{begin_comment};
        while( <FP> ){
            chomp;
            my ($dt,$who,$say) = split(/\t/,$_,3);
            my $text=&enc(&deyen($say)); $text =~ s/\n/<br>/g;
            $buf .= sprintf('%s<span class="commentator">%s</span>
                %s <span class="comment_date">(%s)</span>%s'
                    , $::ss{'begin_comment1'} , &enc(&deyen($who)), $text
                    , &enc($dt) , $::ss{'end_comment1'} );
        }
        $buf .= $::ss{end_comment};
        close(FP);
    }
    unless( exists $opt{f} ){
        $buf .= qq(<form action="$::postme" method="post"
        ><input type="hidden" name="p" value="$etitle"
        ><input type="hidden" name="a" value="comment"
        ><input type="hidden" name="comid" value="$ecomid"
        ><input type="text" name="who" size="10"
        ><input type="text" name="comment" size="60"
        ><input type="submit" name="Comment" value="Comment"></form>);
    }
    $buf . '</div></div>';
}

sub plugin_pagename{
    if( exists $::form{a} && (
        $::form{a} eq 'index'  || $::form{a} eq 'recent' ||
        $::form{a} eq 'rindex' || $::form{a} eq 'older'   )  ){
        'IndexPage';
    }elsif( exists $::form{keyword} ){
        &enc('Seek: '.$::form{keyword});
    }else{
        &enc( exists $::form{p} ? $::form{p} : 'FrontPage' );
    }
}

sub plugin{
    my $session=shift;
    my ($name,$param)=(split(/\s+/,shift,2),'');
    $session->{argv} = $param;

    if( exists $::inline_plugin{$name} ){
        $::inline_plugin{$name}->($session,split(/\s+/,$param)) || '';
    }else{
        'Plugin not found.';
    }
}

sub cr2br{
    my $s=shift;
    $s =~ s/\n/\n<br>/g;
    $s =~ s/ /&nbsp;/g;
    $s;
}

sub preprocess{
    my ($text,$session) = @_;
    my $attachment = $session->{attachment};

    ## >> style
    $text =~ s|&gt;&gt;\{([^\}]+)\}|&inner_link(&verb($1),$1)|ge;

    ## [[...]] style
    $text =~
    s!\[\[(?:([^\|\]]+)\|)?(.+?)\]\]!
        &inner_link(defined($1)?$1:&verb($2),$2)!ge;

    ### http://...{ ... } style
    $text =~ s!($::RXURL)\{([^\}]+)\}!
        &verb(sprintf('<a href="%s"%s>',$1,$::target)).$2.'</a>'!goe;

    ### [...|http://...] style
    $text =~ s!\[([^\|]+)\|((?:\.\.?/|$::PROTOCOL://)[^\]]+)\]!
        &verb(sprintf('<a href="%s"%s>',$2,$::target)).$1.'</a>'!goe;

    ### attachment ###
    $text =~ s|&lt;&lt;\{([^\}]+)\}|
        &verb( exists $attachment->{$1}
        ? $attachment->{$1}->{tag} : "<blink>$&</blink>" )|ge;

    ### tag listed white list ###
    $text =~ s!&lt;(/?(b|big|br|cite|code|del|dfn|em|hr|i|ins|kbd|q|s|samp|small|span|strike|strong|sup|sub|tt|u|var)\s*/?)&gt;!<$1>!gi;

    ### text decorations ###
    $text =~ s|^//.*$||mg;
    $text =~ s|&#39;&#39;&#39;&#39;(.*?)&#39;&#39;&#39;&#39;|<big>$1</big>|gs;
    $text =~ s|&#39;&#39;&#39;(.*?)&#39;&#39;&#39;|<strong>$1</strong>|gs;
    $text =~ s|&#39;&#39;(.*?)&#39;&#39;|<em>$1</em>|gs;
    $text =~ s|__(.*?)__|<u>$1</u>|gs;
    $text =~ s|==(.*?)==|<strike>$1</strike>|gs;
    $text =~ s|``(.*?)``|'<tt class="pre">'.&cr2br($1).'</tt>'|ges;

    #### plugin ###
    $text =~ s/\(\((.+?)\)\)/&plugin($session,$1)/ges;

    ### raw url
    $text = " $text";
    $text =~ s/([^-\"\>\w\.!~'\(\);\/?\@&=+\$,%#])($::RXURL)/
        $1.&verb(sprintf('<a href="%s"%s>',$2,$::target)).$2.'<\/a>'/goe;
    substr($text,0,1)='';
    $text;
}

sub midashi{
    my ($depth,$text,$session)=(@_);
    $text = &preprocess($text,$session);
    my $section = ($session->{section} || ($session->{section}=[0,0,0,0,0])) ;

    if( $depth < 0 ){
        &puts( "<h1>$text</h1>" );
    }else{
        grep( $_ && &puts('</div></div>'),@{$section}[$depth .. $#{$section}]);
        $section->[ $depth ]++;
        grep( $_ = 0 , @{$section}[$depth+1 .. $#{$section} ] );

        my $tag = join('.',@{$section}[0...$depth]);
        my $h    = $depth+( &is('cssstyle') ? 3 : 2 );
        my $cls  = ('sub' x $depth).'section' ;

        $text =~ s/^\+/${tag}. /;
        $text = sprintf( '<a href="%s#p%s" class="%smark sanchor">%s</a>%s'
                , &title2url( $session->{title} )
                , $tag
                , $cls
                , &enc($::config{"${cls}mark"})
                , $text );

        if( ! &is('cssstyle') || $session->{main} ){
            &puts(qq(<div class="${cls} x${cls}">));
        }else{
            &puts(qq(<div class="x${cls}">));
        }
        if( $session->{index} ){
            &puts( qq(<h$h id="p$tag">$text</h$h>) );
        }else{
            &puts( qq(<h$h>$text</h$h>) );
        }
        if( !&is('cssstyle') || $session->{main} ){
            &puts(qq(<div class="${cls}body x${cls}body">));
        }else{
            &puts(qq(<div class="x${cls}body">));
        }
    }
}

sub syntax_engine{
    my ($html,$session) = @_;
    $::syntax_engine->( ref($html) ? $html : \$html , $session );
    &plugin_footnote_flush( $session );
}

sub default_syntax_engine{
    my ($ref2html,$session) = @_;
    my $anchor_count=0;

    &verbatim( $ref2html );

    foreach my $fragment( split(/\r?\n\r?\n/,$$ref2html) ){
        if( $fragment =~ /\A\s*[\*\+]/ ){
            my @stack;
            foreach ( split(  /\n[ \t]*(?=[\*\+])/
                            , &preprocess($fragment,$session) ) )
            {
                my ($mark,$text)=(/\A\s*(\*+|\++)/ ? ($1,$') : ('',$_) );
                my $nest=length($mark);
                my $diff=$nest - scalar(@stack);
                if( $diff > 0 ){### more deep ###
                    if( $mark =~ /\+/ ){
                        &puts( '<ol><li>' x $diff );
                        push( @stack,('</li></ol>') x $diff );
                    }else{
                        &puts('<ul><li>' x $diff );
                        push( @stack,('</li></ul>') x $diff );
                    }
                    $nest > 0 and &puts( $text );
                }else{
                    $diff < 0    and &puts( reverse splice(@stack,$nest) );
                    $#stack >= 0 and &puts( '</li>' );
                    $nest > 0    and &puts( "<li>${text}" );
                }
            }
            &puts( reverse @stack );

        }elsif( $fragment =~ /\A\s*\:/ ){
            ### <DL>...</DL> block ###
            my @s=split(/\n\s*:/, &preprocess($',$session) );
            &puts('<dl>',
                   map( /^:/ ? "<dd>$'</dd>\r\n" : "<dt>$_</dt>\r\n" , @s) ,
                  '</dl>');
        }elsif( $fragment =~ /\A\s*((?:\&lt;){2,6})(.*?)(?:\&gt;){2,6}\s*\Z/s ){
            ### <Hn>...</Hn> block ###
            &midashi( length($1)/4-2 , $2 , $session );
        }elsif( $fragment =~ /\A\s*(\!{1,4})(.*)\Z/s ){
            &midashi( 3 - length($1) , $2 , $session );
        }elsif( $fragment =~ /\A\s*\&gt;&gt;\s*(.*)\s*\&lt;\&lt;\s*\Z/s ){
            my $s=&preprocess($1,$session);
            &puts('<p class="centering" align="center">',$s,'</p>');
        }elsif( $fragment =~ /\A&quot;&quot;/s ){
            $fragment =~ s/^&quot;&quot;//gm;
            &puts('<blockquote>'.&preprocess($fragment,$session).
                    '</blockquote>' );
        }elsif( $fragment =~ /\A\s*\|\|/ ){
            &puts('<table>');
            foreach my $tr ( split(/\|\|/,&preprocess($',$session) ) ){
                my $tag='td';
                if( $tr =~ /\A\|/ ){
                    $tag = 'th'; $tr = $';
                }
                &puts( '<tr>',map("<${tag}>$_</${tag}>",split(/\|/,$tr) ),
                        '</tr>' );
            }
            &puts('</table>');
        }elsif( $fragment =~ /\A\s*&lt;(blockquote|center)&gt;(.*)&lt;\/\1&gt;\s*\Z/si ){
            &puts( "<$1>",&preprocess($2,$session),"</$1>" );
        }elsif( $fragment =~ /\A\s*\-\-\-+\s*\Z/ ){
            &puts( '<hr class="sep">' );
        }elsif( (my $s = &preprocess($fragment,$session)) !~ /^\s*$/s ){
            if( $s =~ /\A\s*<(\w+).*<\/\1[^\/]*>\s*\Z/si ){
                &puts( "<div>${s}</div>" );
            }else{
                &puts("<p>${s}</p>");
            }
        }
    }
    exists $session->{section} and
        grep( $_ && &puts('</div></div>'),@{$session->{section}} );
}
__END__
<!-- Generated with wifky (c) 2005-2006 HAYAMA_Kaoru. -->
