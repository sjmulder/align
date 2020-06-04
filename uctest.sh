#!/bin/sh

# Unicode test
#
# First output a text of known width with no special characters. Then
# output the same text with a special character. It should be aligned
# the same way if the characters are properly counted. The last
# variation involves a grapheme, a character made out of several
# codepoints. This too, should count as a single character.

echo "Romeo & Juliet" | ./center -w20 # ASCII
echo "Romeo & Juliët" | ./center -w20 # UTF-8 NFC (one char)
echo "Romeo & Juliët" | ./center -w20 # UTF-8 NFD (two chars)
