import random
import calendar
import time
from ast import literal_eval

alphabet = "' !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~'}"

letters = "abcdef"


def randomize(string, seed):
    random.seed(seed)
    return ''.join(random.sample(string, len(string)))


def level_One(message, shuffled_alphabet):
    lvl_one = []
    for i in range(0, len(message)):
        index = alphabet.find(message[i])
        lvl_one.append(shuffled_alphabet[index])
    return "".join(lvl_one)


def randomChars(num):
    arr = []
    for i in range(0, num):
        index = random.randint(0, len(alphabet) - 1)
        arr.append(alphabet[index])
    return "".join(arr)


def level_Two(message, random_characters):
    lvl_two = [randomChars(random_characters)]
    for char in message:
        lvl_two.append(char)
        lvl_two.append(randomChars(random_characters))
    return "".join(lvl_two)


def encrypt(message):
    seconds = calendar.timegm(time.gmtime())
    shuffled_alphabet = randomize(alphabet, seconds)
    print("Initial message: " + message)
    lvl_one_msg = level_One(message, shuffled_alphabet)

    random_char_amount = random.randint(1, 6)
    chance = random.randint(1, 100)
    letter = []
    if chance < 50:
        letter.append(letters[random_char_amount - 1])
    else:
        letter.append(letters[random_char_amount - 1].upper())

    lvl_two_msg = level_Two(lvl_one_msg, random_char_amount)

    timestamp_hex = hex(seconds)[2:10]
    final_message = [lvl_two_msg]
    final_message.insert(0, timestamp_hex)
    final_message.insert(0, letter[0])
    return "".join(final_message)


def decrypt(message):
    chars = letters.find(message[0].lower()) + 1
    timestamp = literal_eval("0x"+message[1:9])

    shuffled_alphabet = randomize(alphabet, timestamp)
    msg = []
    for i in range(9 + chars, len(message), chars + 1):
        msg.append(message[i])
    final = []
    for char in msg:
        final.append(alphabet[shuffled_alphabet.find(char)])
    return "".join(final)


# driver code
enc_msg = encrypt("hellooo")
print("Encrypted messgae: " + enc_msg)
dec_msg = decrypt(enc_msg)
print("Decrypted messgae: " + dec_msg)
