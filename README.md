# SequenceAlignment
## Overview
This project implements dynamic programming solution for Sequence Alignment problem.
Given two strings X and Y, we seek to match the characters in these two strings with the cost of the matching is minimized.
## Matching
A matching is a set of ordered character pairs with the property that each item occurs in at most one pair, and there are no crossing pairs.
A gap is a position of X or Y not matched in a matching M so we need to incur a cost δ.
Lining up two different characters p and q is a mismatch, we need to incur a cost α<sub>pq</sub>.
The cost of a matching is the sum of all the gap and mismatch costs in this matching.
## δ and α Values
We use 30 for δ.<br>
| α   | A   | C   | G   | T   |
| --- | --- | --- | --- | --- |
| A   | 0   | 110 | 48  | 94  |
| C   | 110 | 0   | 118 | 48  |
| G   | 48  | 118 | 0   | 110 |
| T   | 94  | 48  | 110 | 0   |
## Input Strings
Input file contains two base strings and their respective insertion positions. For example,<br>
TACG<br>
1<br>
2<br>
9<br>
The string generation process is,<br>
TACG<br>
TA***TACG***CG<br>
TAT***TATACGCG***ACGCG<br>
TATTATACGC***TATTATACGCGACGCG***GACGCG<br>
## Output
Output filename will be output.txt, and the format is as following,<br>
1st line: first 50 characters of first string matching, last 50 characters of first string matching<br>
2nd line: first 50 characters of second string matching, last 50 characters of second string matching<br>
3rd line: matching cost<br>
4th line: time spent<br>
5th line: memory required<br>
## Others
To compile: ***g++ -std=c++11 SequenceAlignmentDp.cpp***<br>
To run: ***./a.out [fileName.txt]***<br>
