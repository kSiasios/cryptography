<?php

$alphabet = " !\"#$%&()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~}";

$letters = "abcdef";

function randomize($string, $seed)
{
    srand($seed);
    $str = str_shuffle($string);
    return $str;
}

function level_One($message, $alphabet, $shuffled_alphabet)
{
    $lvl_one = "";
    $message_array = str_split($message);
    $i = 0;
    foreach ($message_array as $char) {
        $index = strpos($alphabet, $char);
        $lvl_one[$i] = $shuffled_alphabet[$index];
        $i++;
    }
    return $lvl_one;
}

function randomChars($num, $alphabet)
{
    $i = 0;
    $arr = "";
    for ($i = 0; $i < $num; $i++) {
        $index = rand(0, strlen($alphabet) - 1);
        $arr[$i] = $alphabet[$index];
    }
    return $arr;
}

function level_Two($message, $random_characters, $alphabet)
{
    echo "\n\nMessage input: " . $message;
    $lvl_two = randomChars($random_characters, $alphabet);
    $arr = str_split($message);
    foreach ($arr as $char) {
        $lvl_two = $lvl_two . $char;
        $lvl_two = $lvl_two . randomChars($random_characters, $alphabet);
    }
    return $lvl_two;
}

function encrypt($message, $alphabet, $letters)
{
    $seconds = time();
    $shuffled_alphabet = randomize($alphabet, $seconds);
    echo "Initial message: " . $message . "\n";
    $lvl_one = level_One($message, $alphabet, $shuffled_alphabet);
    echo "********\n\n\nLevel One: " . $lvl_one . "\n\n\n********";

    $random_char_amount = rand(1, 6);
    $chance = rand(1, 100);
    $letter = "";
    if ($chance < 50) {
        $letter = $letter . $letters[$random_char_amount - 1];
    } else {
        $letter = $letter . strtoupper($letters[$random_char_amount - 1]);
    }

    $lvl_two = level_Two($lvl_one, $random_char_amount, $alphabet);
    echo "\n\nLevel two: " . $lvl_two . "\n\n";

    $timestamp_hex = dechex($seconds);
    // $final_message = "";
    $final_message = $letter . $timestamp_hex . $lvl_two;
    return $final_message;
}

function decrypt($message, $alphabet, $letters)
{
    $chars = strpos($letters, strtolower($message[0])) + 1;
    $timestamp = hexdec(substr($message, 1, 8));

    $shuffled_alphabet = randomize($alphabet, $timestamp);
    $msg = "";
    for ($i = 9 + $chars; $i < strlen($message); $i = $i + $chars + 1) {
        $msg = $msg . $message[$i];
    }
    $final = "";
    $arr = str_split($msg);
    foreach ($arr as $char) {
        $final = $final . $alphabet[strpos($shuffled_alphabet, $char)];
    }
    return $final;
}

$message = "hellooo";

$enc_msg = encrypt($message, $alphabet, $letters);
echo "\n\n\nEncrypted message: " . $enc_msg;
$dec_msg = decrypt($enc_msg, $alphabet, $letters);
echo "\n\n\nDecrypted message: " . $dec_msg;
