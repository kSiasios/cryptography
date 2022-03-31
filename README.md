# cryptography
Updates on a custom cryptography algorithm developed from the ground up.

I have no prior knowledge on the topic of cryptography, but I wanted to get my hands on it. 
So, I created a custom encryption method in C (and eventually in Python), which is the progarmming language 
I'm most familiar with. I might add other iterations in different languages in the future.

The Article:

## Encrypting data from the ground up

As we move towards a society that is highly based on the world wide web and our online presence is getting larger, privacy problems are getting more serious. So, I decided to try and create an algorithm that will encrypt a string of characters, based on a provided encryption key. Just to make it clear, I am not by any means a professional at security issues and I have no prior deep knowledge on the topic of encryption algorithms. Nevertheless, I have a basic understanding of how you can feed a message to a function so that the output is nothing of value to an attacker or someone who does not have access to the encryption key.

### Initial thoughts and encryption baseline

The most basic way, as far as I can understand, of encrypting a message is to shift the characters X positions to the right or to the left on the provided alphabet. For example, if our alphabet is the Latin one and our initial message is “hello”, we can shift our alphabet and have a completely different message in the end of the process. Shifting our alphabet 7 positions to the left will give us a string of “olssv”.

| A | B | C | D | `E` | F | G | `H` | I | J | K | `L` | M | N | `O` | P | Q | R | S | T | U | V | W | X | Y | Z |
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| H | I | J | K | `L` | M | N | `O` | P | Q | R | `S` | T | U | `V` | W | X | Y | Z | A | B | C | D | E | F | G |

This is a fairly simple way to encrypt messages but they can be decrypted fairly easily with a simple brute-force attack. We can take it a step further by not just shifting the alphabet, but jumbling the letters around. Using the previous example and the table below, which is a random sequence I came up with, the output would be “ebuug”.

| A | B | C | D | `E` | F | G | `H` | I | J | K | `L` | M | N | `O` | P | Q | R | S | T | U | V | W | X | Y | Z |
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| S | P | R | J | `B` | W | K | `E` | H | Z | M | `U` | C | O | `G` | Q | T | I | D | L | N | A | Y | F | V | D |

At a first glance, the message is gibberish as was the previous one. But there is no obvious pattern on how the letters are arranged. Even though there are no specific patterns, computers are extremely talented at spitting results fast. You can understand this better on a smaller scale. If our alphabet is “abc” and the message is “cabba”, then, provided the jumbled table, the encrypted message would be “abccb”. It would take a computer 6 tries to find all the jumbled alphabets included the initial one and provide us with all the possible combinations. The result is given by the sum of all the numbers starting form the number given by subtracting the number of positions from the total of the possible values, spanning to the total of the possible values. In our case, the sum of the number from (3 possible values – 3 positions (3-lettered alphabet)) to the 3 (possible values). Which gives us 0+1+2+3=6.

Σ~(n=total - positions)~n


| A | B | C |
| ----------- | ----------- | ----------- |
| B | C | A |

There are two more things making it easier for a decoder to crack our message. The first one is that there are repeating characters in our message and we transfer this to our encoded message. So, using this method, the letter “l” will always be “u” in the resulting string, which is a bad practice for encrypting messages. The second one is the fact that whatever our message is, the final message will have the same length. The string “hello” and the string “ebuug” both have a length of 5 characters which again might be a giveaway of our message. So, we can add random characters between each pair of characters of our message. The result could look something like that: `“aGekIbfeuloupogDt”`. 

### Randomizing the amount of random junk.

The previous example uses 2 random characters between the letters. But the amount of random alphanums could also be random. To do so, and to actually be able to decode our message, we can add a header or a tail to the encoded string. A header (or a tail), is an extra string of data that holds information valuable to the decoder. In our case, the header can contain the amount of characters we need to ignore to get to a valid character. Or, in other words, how many random characters there are between our message. We can also add another piece of data which will allow the decoder to produce the alphabet in real time, which will ensure that even if a message is decoded once, the next one will have a different encryption. Obviously, we need to also encode our header and tail in some way. 

The easiest one, since we want to encode a number and we are working with characters, would be to convert our number to hexadecimal and add a fixed number, for example 9. This way, if we had 2 random characters, adding 9 to it and converting 11 to a hex number would return B. In the decoder function, we reconvert from hexadecimal to decimal giving us 11 and we subtract our fixed number (9), giving us the number of random characters. Now, this can only work with a maximum of 6 random characters, since adding 9 to 7 would result in 16 which in its hexadecimal form is 10, adding one more character and making our job harder, since we don’t know how many characters to read, to get the value we want.

### Writing the actual functions 

All of this seems good on paper. But what algorithm do we use to produce the alphabet? This algorithm must return the same result given a particular input so that the decoder in the destination can reproduce the alphabet. The way that I am going to achieve that is by using the Fisher-Yates shuffle Algorithm found in GeeksforGeeks. I’m going to be coding these examples in C, which is the language I’m the most familiar with. As stated earlier, we are going to pass two thing in our header. The number of random characters and a seed for the shuffling function. This seed will be the timestamp at which our message was created and it will be fed to the srand() function in place of time(NULL) that we usually put as an argument. This will ensure that the shuffling of the ASCII table is random and dependent on time and, additionally, we will be able to reproduce the result. We are also going to be using a part of the ASCII table for ease starting from 32 (space character) all the way to 126 (~). This means that we need a table of size 95 to save the original ASCII table and another one with the same size that will contain the shuffled version. 

Next up, I will create the two functions, encrypt() and decrypt(), to demonstrate the effect,along with some other helping functions to make the code easier to read. Starting from encrypt, this function will firstly initialize the secondary table copying the values of the original one and randomize the table to produce the first encrypted string. The Function level_One() will be the one to return the shuffled version of our message. Using the string “hello” in the example below, the function returns `“2#gg>”`

Iterating through each one of the message characters, this function finds the index of the character on the ASCII table and points to the same index on the shuffled one. On the next level of encryption, we need to add random characters between the message characters. A function that returns two random characters should be created, since it is going to be used a lot throughout this level. Given an input of 10, which is the number of characters we want, it produces random sequences of ten characters. If we give it the string “hellooo” and set the amount of random characters to 3, it will produce the string `“hadhgwlehadlw?/lw%/os\5oT[{o\S)”`. As you can see, it already is unrecognizable to humans, even without level_One().

The next and final step will be to randomize the amount of random characters, and add it along with the timestamp to the message header. For that we will need a function to convert decimal numbers to hexadecimal. Again GeeksforGeeks comes to the rescue. 

** A few days of troubleshooting later, I’m limiting the amount of random characters to 5, since when the variable gets a value of six, a bug appears that I’ve yet to find a solution for. If anything changes in the future, I’ll give you an update. **

We will next append the returned hex, as well as the encrypted message returned from level_Two(), to the letter that corresponds to the amount of random numbers. For this, I’ll just use strcat(), which is a function that concatenates two strings and it is built in the string.h library.

As you can see below, the function is working great and returns a random string each time.

Now deciphering the message is a matter of tokenizing the string and reverse procedure. First things first, we need to know how many random characters there are between the valid characters of the encrypted message. We’ ll take the first character and feed it to a function that converts the read character to an integer. Next up, we have to read and convert the timestamp from hexadecimal, to decimal form to be able to recreate the shuffled version of our alphabet. 

Then, we skip X characters, where X is the amount of the random characters that we decoded earlier, and save the valuable characters to a string. At this point, we have successfully reverted the level_Two() encryption and we are left with the level_One() encrypted version of the message. The last thing to do, is to shuffle the alphabet using the timestamp (that we, once again, decoded earlier) as a seed to the randomize() function and match the shuffled version of the message to the correct one. 

And, it’s done. We can now encrypt and decrypt a message so that, in case of an intrusion, the attacker will have no idea of what the message means without the proper decoder. We can actually make it even harder to decode by jumbling the header inputs around, e.g. put the timestamp at the 7th position from the start instead of the 1st, but this would mean that my sanity would completely drain out, since there are still some bugs laying around in the code.

The next step would be to test it on a real life scenario, like a chat session in a live server. But this is a topic for another time, so stay tuned for that.
