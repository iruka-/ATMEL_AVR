#!/usr/bin/perl

#-----------------------------------------
#				dir.cgi
#-----------------------------------------

#
#	カレントディレクトリの一覧を表示する.
#


#  主要関数
#    @filelist = read_dir( $dir ) 指定ディレクトリのファイル名リストを返す.
#                                 再帰下降無し
#								  '.' '..' は取り除かれる.
#								  ファイル名(パス名付属しない)のリストが返る.
#
use strict;
my  %input;
my $buffer;
###########################################################################
#    @filelist = read_dir( $dir ) 指定ディレクトリのファイル名リストを返す.
#                                 再帰下降無し、ディレクトリ含む.
###########################################################################
sub	read_dir() {
	my ($dir)=@_;

	opendir(FILE,$dir);
	my @filelist = readdir(FILE);
	closedir(FILE);

    if($filelist[0] eq '.') {
       shift @filelist;
    }
    if($filelist[0] eq '..') {
       shift @filelist;
    }

	return @filelist;
}

#-----------------------------------------
#	CGIパラメータを受け取って連想配列に入れる.
#-----------------------------------------
sub read_input()
{
	my (@pairs, $pair, $name, $value, %FORM);
	$ENV{'REQUEST_METHOD'} =~ tr/a-z/A-Z/;
	if ($ENV{'REQUEST_METHOD'} eq "POST") {
		read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
	}else{
		$buffer = $ENV{'QUERY_STRING'};
	}

	@pairs = split(/&/, $buffer);
	foreach $pair (@pairs) {
		($name, $value) = split(/=/, $pair);
		$value =~ tr/+/ /;
		$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;

#		&jcode'convert(*value, 'euc'); #'

		$value =~ s/\&/\&amp\;/ig;
		$value =~ s/\"/\&quot\;/ig; #"
		$value =~ s/\</\&lt\;/ig;
		$value =~ s/\>/\&gt\;/ig;

        $FORM{$name} = $value;
    }
    %FORM;
}

#-----------------------------------------
#
#-----------------------------------------
sub	html_header()
{
	print "Content-type: text/html; charset=Shift_JIS\n\n";
#	print "charset=Shift_JIS\n\n";

	print "<html>\n";
	print "<body>\n";
}

#-----------------------------------------
#
#-----------------------------------------
sub	html_footer()
{
	print "</body>\n";
	print "</html>\n";
}

#-----------------------------------------
#
#-----------------------------------------
sub	print_input()
{
	my ($key,$val);
	print "<pre>\n";
	foreach $key (keys %input) {
		$val = $input{$key};
		print "$key = $val\n";
	}
	print "</pre>\n";
}

#-----------------------------------------
#
#-----------------------------------------
sub	cap2sjis()
{
	my ($file) = @_;
	my $sjis='';
	my ($i,$c,$h);

	for($i=0;$i<length($file);$i++) {
		$c = substr($file,$i,1);
		if($c eq ':') {
			$h = substr($file,$i+1,2);$i+=2;
			$c = pack("C", hex($h) );
		}
		$sjis = $sjis . $c;
	}
	return $sjis;
}
#-----------------------------------------
#
#-----------------------------------------
sub	printfileinfo()
{
	my ($file) = @_;
	my ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks)
		= stat $file;
}
#-----------------------------------------
#
#-----------------------------------------
sub	printfile()
{
	my ($curdir,$file) = @_;
	my $sjis = &cap2sjis($file);
	my $dir = '';

	my ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks)
		= stat $file;
	my $t = localtime($mtime);
	my $size_k = int( ($size + 1023) / 1024);
	my $color='';
	my $adir='';

	if($file =~ /^\./)   {return;}
	if($file =~ /\.cgi/) {return;}
	if($file =~ /\.jpg/) {return;}
	if($file =~ /\.jpeg/){return;}
	if($file =~ /\.png/) {return;}
	if($file =~ /\.dir/) {return;}
	if($file =~ /\.sh/) {return;}
	if($file =~ /~/)    {return;}

	if($mode & 0x4000) {
		$adir = "<a href=\"index.cgi?dir=$curdir$file\">[dir]</a>";
		$dir = '/'; 
		$color = ' bgcolor=#e0e080';
	}
	print "<tr$color>";
	printf( '<td>%s<a href="%s">%s</a> %s ' . "</td>\n"
		,$adir , $curdir . $file ,   $sjis . $dir);

	print "<td align='right'>$size_k" . "k</td>";

	print "<td>                       </td>";

#	print "<td>$t</td>";
	my @times=split(/[ ]+/,$t);
	my $t1;
	foreach $t1(@times) { 
		print "<td>$t1</td>";
	}
	print "</tr>";
}
sub sanitize()
{
	my ($s) = @_;
	$s =~ s/[ \t\.]+//g;
	return $s;
}
#-----------------------------------------
#
#-----------------------------------------
sub	dirlist()
{
	my ($currentdir) = @_;
	my $file;
	my @list;
	my $subtree = '/' . $currentdir;

	$currentdir = &sanitize($currentdir);

	my $subdir = $currentdir;
	if($subdir eq '') {
		$subdir = '/';
	}else{
		$currentdir .= '/';
	}

	my $pwd =`pwd`;
	$pwd =~ s/\n//;

#	printf "<h2>Index of $pwd$subtree</h2><hr>\n";
	printf "<h2>Index of $pwd/$subdir</h2><hr>\n";

	my @filelist = &read_dir($pwd . '/' . $subdir);
	@list = sort(@filelist);


	print "<table>";
	foreach $file (@list) {
		&printfile($currentdir , $file);
	}
	print "</table>";

	print "<hr>";
	print "Created by filelist.cgi"
#	print "この一覧はindex.cgiが作成しました。";
#	print " [dir] をクリックするとhttpd::Indexesのかわりにindex.cgiが表\示します。"
}

#-----------------------------------------
#
#-----------------------------------------
sub	main()
{
	my $cmd;
	%input = &read_input();
	&html_header();
	&dirlist($input{'dir'});
	&html_footer();
}

#-----------------------------------------
#
#-----------------------------------------
#
&main();
1;
#
