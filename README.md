# SimpleWeakCryptoTool
Very simple tool to encrypt, encrypt, freq analyze, and attack(crack).  Meant as a way to demonstrate simple substitution encryption and how to break it.

Command Line Usage:
Encrypt
Week6 -e -kxxxxxxx -plaintextfile.txt -cipherfile.txt
Decrypt 
Week6 -d -kxxxxxx -cipherfile.txt -plaintextfile.txt
Frequency Analysis [IN-WORK]
Week6 -f -cipherfile.txt
Attack (Crack Code with Frequency Analysis) [NOT IMPLEMENTED]
Week6 -a -cipherfile.txt -plaintextfile.txt

Notes: -kxxxxxx: replace the X's with a passphrase to generate a cipher key. (i.e. -kSECRETCODE)

Interactive Usage:
Just run the executable.
It will prompt for inputs.
