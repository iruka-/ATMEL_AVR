#!/usr/bin/perl


#
# sine table 
#
my $center = 0;
my $scale  = 128-8;

#my $center = 128/4;
#my $scale  = (128/4 - 4);

my $twopi  = 64;

#
# •½‹Ï—¦‰¹ŠKtable ‚ğ¶¬.
#
sub tone12()
{
	$m = 128.0;
	$r = 2 ** (1/12);
	for($i=0;$i<12*4;$i++) {
		printf("	0x%x,	/* %2d */\n",$m,$i);
		$m = $m * $r;
	}
}

sub main()
{
	&tone12();
}


&main();

