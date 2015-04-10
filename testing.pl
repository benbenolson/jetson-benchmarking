#!/usr/bin/perl

open(my $fh, ">", "data.csv");
print $fh "\"exec\",\"numthreads\",\"time\"\n";
my @numthreads = (
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  20, 30, 40, 50, 100, 300);
my @execs = (
  "bimage_nothread",
  "bimage_thread",
  "bimage_threadpool");
my @images = (
  "images/flowers.bmp" );

foreach my $exec (@execs) {
  unless( -e $exec) {
    print "Cannot find $exec\n";
    die;
  }
  print "Testing $exec:\n";
  foreach my $image (@images) {
    foreach my $numthread (@numthreads) {
      my $output;
      if(($exec eq "bimage_nothread") or ($exec eq "bimage_cuda")) {
        $output = `./$exec $image`;
      } else {
        $output = `./$exec $image $numthread`;
      }
      $output = 1200 / $output;
      print $fh "\"$exec\",\"$numthread\",\"$output\"\n";
    }
  }
}

close($fh);
