def main():
    # Ask the user to insert the text
    txt = input("Text: ")

    # Count the number of letters within the text
    n_let = count_letters(txt)

    # Count the number of words within the text
    n_wor = count_words(txt)

    # Count the number of sentences within the text
    n_sen = count_sentences(txt)

    # Calculate the L (the average number of letters per 100 words in the text)
    L = float((n_let / n_wor) * 100)

    # Calculate the S (the average number of sentences per 100 words in the text)
    S = float((n_sen / n_wor) * 100)

    # The Coleman-Liau index
    index = 0.0588 * L - 0.296 * S - 15.8

    # Round an ugly floated number to a nice integer one, i.e. the user's English grade
    grade = round(index)

    if grade < 1:
        print("Before Grade 1")
    elif (grade >= 1 and grade < 16):
        print(f"Grade {grade}")
    else:
        print("Grade 16+")


def count_letters(text):
    letters = 0
    for char in text:
        if char.isalpha():
            letters += 1

    return letters


def count_words(text):
    spaces = 0
    for char in text:
        if char == " ":
            spaces += 1

    # As described in the "walkthrough" video, the number of the words is generally the number of spaces plus one
    return spaces + 1


def count_sentences(text):
    punctuation = 0
    for char in text:
        # A punctuation is whether a question mark ("?"), a period ('.') or an exclamation point ("!")
        if char == "?" or char == "." or char == "!":
            punctuation += 1

    # Unlike the previous function, the number of punctuations is the number of sentences, once the very last character has to be a period
    return punctuation


if __name__ == "__main__":
    main()

