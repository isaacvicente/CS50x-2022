-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Start with the description from "crime_scene_reports" table
SELECT description FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";

-- There were three witnesses who were at the moment the theft occured, at 10:15am at Humphrey Street bakery:
/*
    (1) Eugene: "I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money."

    (3) Ruth: "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot,
    you might want to look for cars that left the parking lot in that time frame."

    (2) Raymond: "As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
     The thief then asked the person on the other end of the phone to purchase the flight ticket."
*/
SELECT transcript, name FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;

-- (I) Following the first (1) interview, we have eight bank account numbers used to make withdrawings in the Leggett Street at the date the crime occured:
SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND year = 2021 AND month = 7 AND day = 28;

-- (II) Following the second (2) interview, we have eighteen (nine callers and nine receivers) phone numbers in which had made a less than one minute call in the date the theft occured
SELECT caller, receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- (III) Following the third (3) interview, we have eight car numbers which left the bakery within 10 minutes (the theft occured at 10:15am in 28th July of 2021)
SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25;

-- Crossing the information between I and II we have six suspects with their IDs and names:
/*
    +--------+---------+
    |   id   |  name   |
    +--------+---------+
    | 395717 | Kenny   |
    | 438727 | Benista |
    | 449774 | Taylor  |
    | 514354 | Diana   |
    | 686048 | Bruce   |
    | 907148 | Carina  |
    +--------+---------+
*/
SELECT DISTINCT(people.id), people.name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN phone_calls ON phone_calls.caller = people.phone_number WHERE people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND year = 2021 AND month = 7 AND day = 28);

-- Maybe the thief left the bakery with their own car, so let's cross our suspects with the people who left the bakery at the crime's date (III)
-- Now we have two main suspects (given that case):
/*
    +--------+-------+
    |   id   | name  |
    +--------+-------+
    | 514354 | Diana |
    | 686048 | Bruce |
    +--------+-------+
*/
SELECT people.id, people.name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25) AND id IN (SELECT DISTINCT(people.id) FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN phone_calls ON phone_calls.caller = people.phone_number WHERE people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND year = 2021 AND month = 7 AND day = 28));

-- Still following the second (2) interview, we have the destination_airport_id of the earliest flight on 29th July of 2021 (one day after the crime date)
SELECT destination_airport_id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1;
-- Having that id on hand, we discover that the thief escaped to: New York City
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1);

-- We can now see who are the people that flew in that flight
SELECT id, name FROM people JOIN passengers ON people.passport_number = passengers.passport_number WHERE passengers.flight_id = (SELECT id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1);
-- We now have three names that matchs our suspects list [Bruce (the most suspected one), Taylor and Kenny]
-- Crossing our suspect list with the list given by the most recent command right above:
/*
    +--------+--------+
    |   id   |  name  |
    +--------+--------+
    | 395717 | Kenny  |
    | 449774 | Taylor |
    | 686048 | Bruce  |
    +--------+--------+
*/
SELECT name FROM people JOIN passengers ON people.passport_number = passengers.passport_number WHERE passengers.flight_id = (SELECT id FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29 ORDER BY hour, minute LIMIT 1) AND people.id IN (SELECT DISTINCT(people.id) FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN phone_calls ON phone_calls.caller = people.phone_number WHERE people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE transaction_type = "withdraw" AND year = 2021 AND month = 7 AND day = 28));

-- As our suspect list got tigher, we can check, for each suspect (starting with the most one, Bruce), the person who received their call (query II)
-- For Bruce (the most suspect), we find that his receiver in the phone call was Robin:
SELECT people.name FROM people JOIN phone_calls ON phone_calls.receiver = people.phone_number WHERE phone_calls.year = 2021 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration < 60 AND phone_calls.caller = (SELECT phone_number FROM people WHERE name = "Bruce");
-- After test Bruce for the thief and Robin as his accomplice, we solve the mystery. Voilà!
/* So:
    The THIEF is: Bruce
    The city the thief ESCAPED TO: New York City
    The ACCOMPLICE is: Robin
*/

