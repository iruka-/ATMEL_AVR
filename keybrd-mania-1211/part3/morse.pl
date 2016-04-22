#!/usr/bin/perl

sub loadfile()
{
	my ($file)=@_;
	my @buf;
	open(FILE,$file) || die("Can't open file $file");
	@buf = <FILE>;
	close(FILE);
	return @buf;
}

sub morse()
{
	my ($s1)=@_;
	my ($i,$c);
	my $m=length($s1);
	printf("	MORSE(0b");
	for($i=0;$i<5;$i++) {
		$c = substr($s1,$i,1);
		if($c eq '.') {print '1';}
		else {print '0';}
	}
	printf("\t,%d);	// ",$m);
}

sub main()
{
	my @buf = &loadfile('morse.txt');
	my $n=0;
	while(1) {
		my $s1 = $buf[$n+0];
		my $s2 = $buf[$n+1];$n=$n+2;

		if($s1 eq '') {last;}
		$s1 =~ s/\r//;
		$s2 =~ s/\r//;
		$s1 =~ s/\n//;
		$s2 =~ s/\n//;

		&morse($s2);
		print $s1 . $s2 . "\n";
	}
}

&main()

