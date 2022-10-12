from cs50 import get_float


def main():
    # Get how many cents the customer is owed
    cents = get_cents()

    # Calculate the number of quarters to give the customer
    quarters = calculate_quarters(cents)
    cents -= quarters * 25

    # Calculate the number of dimes to give the customer
    dimes = calculate_dimes(cents)
    cents -= dimes * 10

    # Calculate the number of nickels to give the customer
    nickels = calculate_nickels(cents)
    cents -= nickels * 5

    # Calculate the number of pennies to give the customer
    # (which is what we have from cents, after it was updated by nickels)
    pennies = cents

    # Sum coins
    coins = quarters + dimes + nickels + pennies
    # Print total number of coins to give the customer
    print(coins)


def get_cents():
    while True:
        # Ask how many dollars the customer is owed
        n = get_float("Change owed: ")
        if n < 0:
            # Alert the user to type a positive number
            print("Type a positive number!")
            # If the customer did so, break the while loop
        else:
            break
    # Convert dollars to cents (0.41 dollars should be 41 cents)
    n = int(n * 100)
    return n


def calculate_quarters(cents):
    quarters = cents // 25
    return quarters


def calculate_dimes(cents):
    dimes = cents // 10
    return dimes


def calculate_nickels(cents):
    nickels = cents // 5
    return nickels


if __name__ == "__main__":
    main()
