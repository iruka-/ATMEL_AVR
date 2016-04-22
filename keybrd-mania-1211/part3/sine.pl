#!/usr/bin/perl


#
# sine table 
#
my $center = 0;
my $scale  = 128-8;

my $scale1 = 80;
my $scale2 = 60;

my $twopi  = 64;

#
# 	”{‰¹‚ğŠÜ‚Ş sine table ‚ğ¶¬.
#
sub mixwave()
{
	for($i=0;$i<=$twopi;$i++) {
		$rad1 =  $i    * 3.1415926535 * 2.0 / $twopi;
		$rad2 = ($i*2) * 3.1415926535 * 2.0 / $twopi;

		$sine = int( sin($rad1) * $scale1 + sin($rad2) * $scale2 + $center );
		$shex = $sine & 0xff;

		printf("	0x%x,	/* %2d: %3d */\n",$shex,$i,$sine - $center);
	}
}

#
# 	sine table ‚ğ¶¬.
#
sub squarewave()
{
	for($i=0;$i<=$twopi;$i++) {
#		$rad = $i * 3.1415926535 * 2.0 / $twopi;

		if($i < ($twopi/2)) {
			$sine = $scale;
		}else{
			$sine = -$scale;
		}

		$shex = $sine & 0xff;


		printf("	0x%x,	/* %2d: %3d */\n",$shex,$i,$sine - $center);
	}
}

#
# 	sine table ‚ğ¶¬.
#
sub sinewave()
{
	for($i=0;$i<=$twopi;$i++) {
		$rad = $i * 3.1415926535 * 2.0 / $twopi;
		$sine = int( sin($rad) * $scale + $center );
		$shex = $sine & 0xff;
		printf("	0x%x,	/* %2d: %3d */\n",$shex,$i,$sine - $center);
	}
}

sub main()
{
#	&sinewave();
#	&mixwave();
	&squarewave();
}


&main();

