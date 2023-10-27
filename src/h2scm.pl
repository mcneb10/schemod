#!/usr/bin/env perl
use strict;
use warnings;

use Pod::Usage;
use Getopt::Long;

use HeaderParser;

my $library_path = shift @ARGV;
my $output_scheme_file = pop @ARGV;
my @header_files = @ARGV;

# INCOMPLETE

# Get preprocessor defined symbols to figure out if a specific function signature is the right one to use
my @predefined_symbols = split /\R/, `echo | c++ -dM -E -x c++ -`;

my $symbols_text;

if($^O eq 'darwin') {
    $symbols_text = `nm -C "$library_path"`
} else {
    $symbols_text = `nm -DC "$library_path"`
}


my $hp = HeaderParser->new();
my @preprocessor_data = ();
for my $header(@header_files) {
    open my $fh, '<', $header;
    push @preprocessor_data, $hp->parse_fh($fh);
    close $fh;
}

for my $symbol (split /\R/, $symbols_text) {
   if($symbol =~ /^[\da-f]{16} T (.*?) /) {
    my $symbol = $1;
    for $header_file_data (@preprocessor_data) {
        
    }
   } else {
    next;
   }
}

__END__
=head1 NAME
h2scm - convert header files to scheme bindings

=head1 SYNOPSIS

h2scm library_path header_files output_scheme_file

=over 4



=back