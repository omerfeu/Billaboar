# Billaboar
An app for managing shared bills and expenses. 
## Details
The program receives input of everyone's income and expenses and calculates the debt and repayments required from each one. There's an option for calculating the minimal number of repayments required, though this problem is NP-Complete so it's not recommended for over 8 people due to long runtime.
## Usage
Command line execution: [path to Billaboar.exe] [path to input file] [output file]
<br>
This program works with csv input files of the form: 
<br><br>
title
<br>
[name],[payment]
<br><br>
[payment] can be either negative, indicating expense, or positive, indicating income.
<br>
The calculation assumes each payment splits equally between everyone.
### For example:
#### input file:
name:payment
<br>
yossi,-20
<br>
yossi,-30
<br>
menash,-10
<br>
menash,10
#### output file:
Debts:
<br>
menash owes -25
<br>
yossi gets 25
<br><br>
Repayments:
<br>
menash pays yossi 25
