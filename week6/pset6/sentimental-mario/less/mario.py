from cs50 import get_int
# Ensure that the user type a positive value between 1 and 8, inclusive
while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
        break
# A variable we'll play with
n = height
# Iterates through the half-pyramid's height
for i in range(height):
    # Print the spaces, for identation
    print(" " * (n - 1), end="")
    # At each iteration, decrease the number of spaces printed
    n -= 1
    # Print ones pyramid's row
    print("#" * (i + 1), end="")
    # Jump to the next line
    print()

