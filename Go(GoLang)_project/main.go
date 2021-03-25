package main

import (
	"fmt"
	"math/rand"
	"strconv"
	"strings"
	"time"
)

var alphabet = " !\"#$%&()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~'}"

var letters = "abcdef"

func randomize(str string, seed int64) string {
	// randomize Seed
	rand.Seed((int64(seed)))

	// convert string to rune to be able to shuffle it
	strRune := []rune(str)

	// shuffle the rune
	rand.Shuffle(len(strRune), func(i, j int) {
		strRune[i], strRune[j] = strRune[j], strRune[i]
	})

	// convert it back to string and return it
	return string(strRune)
}

func level_One(message string, shuffled_alphabet string) string {
	// declare a variable to save the output
	var lvl_one string
	// iterate through all the characters of the input message
	for letter := range message {
		// get the index of each character on the provided alphabet
		index := strings.Index(alphabet, string(message[letter]))
		// append the character in the same index in the shuffled alphabet
		// to the output message
		lvl_one += string(shuffled_alphabet[index])
	}
	// return the output of the algorithm
	return string(lvl_one)
}

func randomChars(num int) string {
	// declare a string to save the random characters
	arr := ""
	// iterate for each character
	for i := 0; i < num; i++ {
		// produce a random index from 0 to the length of the alphabet
		index := rand.Intn(len(alphabet))
		// append the character at the returned index in the alphabet, to
		// our output variable
		arr += string(alphabet[index])
	}
	// return the final string
	return arr
}

func level_Two(message string, random_characters int) string {
	// start by declaring a string to save the output
	// then initialize it with N amount of random characters
	lvl_two := randomChars(random_characters)
	// iterate through each character
	for i := range message {
		// append the current character to the output
		lvl_two += string(message[i])
		// append N amount of random characters to the output
		lvl_two += randomChars(random_characters)
	}
	// return the final string
	return lvl_two
}

func encrypt(message string) string {
	// initialize the seed using the current time in seconds
	seed := time.Now().Unix()
	// shuffle the alphabet and save it to a variable
	shuffled_alphabet := randomize(alphabet, seed)
	// produce the first level of encryption
	lvl_one := level_One(message, shuffled_alphabet)
	// the amount of random characters between each pair of characters (1 to 6)
	random_char_amount := rand.Intn(6) + 1
	// the chance of the letter being upper case or lower case
	chance := rand.Intn(100)
	// variable to save the letter in
	letter := ""
	// 50% chance to save the lower case letter
	if chance < 50 {
		letter += string(letters[random_char_amount-1])
	} else {
		letter += strings.ToUpper(string(letters[random_char_amount-1]))
	}
	// produce the second level of encryption
	lvl_two := level_Two(lvl_one, random_char_amount)

	// the chance of the hex being upper case or lower case
	chance = rand.Intn(100)
	// 50% chance to save the lower case hex

	timestamp_hex := fmt.Sprintf("%x", seed)
	if chance >= 50 {
		timestamp_hex = strings.ToUpper(timestamp_hex)
	}

	final_message := letter + timestamp_hex + lvl_two
	return final_message
}

func decrypt(message string) string {
	// get the first character and translate it to an integer
	chars := strings.Index(letters, strings.ToLower(string(message[0]))) + 1
	// read the hex timestamp and convert it to int
	timestamp, err := strconv.ParseUint(message[1:9], 16, 64)
	// check for errors at the conversion  process
	if err != nil {
		fmt.Println(err)
		return "Error in timestamp conversion!"
	}
	// recreate the shuffled version of the alphabet
	shuffled_alphabet := randomize(alphabet, int64(timestamp))
	// declare a variable to save the level two decryption
	msg := ""
	// skip every N amount of characters (junk) and save valuable characters
	for i := 9 + chars; i < len(message); i = (i + chars + 1) {
		msg += string(message[i])
	}
	// declare a variable to save the level one decryption
	final := ""
	// iterate through the returned decrypted message and match each character to
	// its corresponding one on the unshuffled alphabet
	for i := range msg {
		final += string(alphabet[strings.Index(shuffled_alphabet, string(msg[i]))])
	}
	// return the decrypted message
	return final
}

func main() {
	msg := "hellooo"
	println("Initial message: " + msg)

	println()
	enc_msg := encrypt(msg)
	println("Encrypted message: " + enc_msg)

	println()
	dec_msg := decrypt(enc_msg)
	println("Decrypted message: " + dec_msg)
}
