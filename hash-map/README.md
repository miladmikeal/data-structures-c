# Concordance - Hash Map

The concordance counts how many times each word (case-insensitive) occurs in a document. Each hash link in the table stores a word from the document as the key and the number of times the word appears as the value.

## Compile and run concordance

    make all
    ./prog <filename>

Defaults to input1.txt if no file is provided

# Spellchecker - Hash Map

The program loads the 'dictionary.txt' file. The user inputs a word,; if the word is spelled correctly, the user is notified that the word is spelled correctly. Otherwise, the program uses the Leveshtein Distance algorithm to calculate the 5 closest words to the word that was spelled incorrectly.

## Compile and run spellchecker

    make all
    ./spellChecker

## Compile and run tests

    make all
    ./tests

## Clean

    make clean
