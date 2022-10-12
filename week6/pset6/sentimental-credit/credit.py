from cs50 import get_string, get_int

while True:
    # Get a credit card number from the user while he/she doesn't cooperate
    number = get_int("Number: ")
    if number < 0:
        print("The number must be an integer!")
    else:
        break

# Initialize to zero some varibles we will need
last = digits = sum_of_odds = sum_of_evens = 0

# The number will decrease until it becomes a float type number, so the result of 'number / 10' will
# be zero, since, for instance, with integers, 1 / 10 = 0
while number > 0:
    # We are "reading" the number from right to left, so the first number is the last one, and so on.
    digits += 1

    # Store the second last digit for the end
    sec_last = last

    # This gives us the last digit of the number
    last = number % 10

    # The second-to-last digits (i.e, the second, forth, sixth... from right to left)
    if digits % 2 == 0:
        # Do the Luhn’s Algorithm
        multiplier = last * 2

        if multiplier >= 10:
            n1 = multiplier // 10
            n2 = multiplier % 10
            multiplier = n1 + n2

        sum_of_evens += multiplier
    else:
        # So we have the odd numbers, i.e., the first, third, fifth, etc, from right to left
        sum_of_odds += last

    # Now that we have the last digit, lets reduce the number by one digit (e.g. 123 becomes 12)
    number = number // 10

# Sometimes its required to know the 'firsts' digits of the whole number, i.e the first and the second one
# from left to right
firsts = (last * 10) + sec_last

# Sum the sums
total_sum = sum_of_evens + sum_of_odds

# That final sum must have the last digit as been a zero, i.e 10, 50 or 80 would be a valid value, whereas 89 not
if total_sum % 10 == 0:
    # Check for each credit card identification and then print it's name, e.g., MASTERCARD
    if (digits == 13 or digits == 16) and last == 4:
        print("VISA")
    elif digits == 15 and (firsts == 34 or firsts == 37):
        print("AMEX")
    elif digits == 16 and (firsts >= 51 and firsts <= 55):
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")

