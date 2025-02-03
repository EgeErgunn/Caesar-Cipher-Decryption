#include <stdio.h>
#include <string.h>

char toUpper(char c) // Convert char to upper case
    {
    if (c >= 'a' && c <= 'z') // Check if the char is an lowercase letter
            {
	    return c - 32;   // Subtract 32
            }
    return c;  // Return the character unchanged if it's not lowercase
    }

char toLower(char c) // Convert char to lower case
    {
    if (c >= 'A' && c <= 'Z') // Check if the char is an uppercase letter
              {  
              return c + 32;  // Add 32 to convert to lowercase
              } 
    return c;  // Return the character unchanged if it's not uppercase
    }

void caeserDecryption() 
{
    int shiftAmount1 = DecrypteTheMessage("enemy_messages/message1_encrypted.txt");
    PrintTheMessageWithShifted("enemy_messages/message1_encrypted.txt", shiftAmount1);

    int shiftAmount2 = DecrypteTheMessage("enemy_messages/message2_encrypted.txt");
    PrintTheMessageWithShifted("enemy_messages/message2_encrypted.txt", shiftAmount2);

    int shiftAmount3 = DecrypteTheMessage("enemy_messages/message3_encrypted.txt");
    PrintTheMessageWithShifted("enemy_messages/message3_encrypted.txt", shiftAmount3);
}

// This function find how many shift does it needs in order to be decrypted, it just not try shifting alphabetically, it sort alphabet by the appereances in the sentence, and then assuming the most apperaed letter as the letter 'e'
int DecrypteTheMessage(char *fileName)
{
    FILE *encryptedMessagesFile = fopen(fileName, "r"); // Open the file that contains decrypted sentences

    char commonWordsWithE[] = "the are and be you we she he not that have very me for with this"; // Commons words in English, this array help to find the correct shifting
    char sentence[300]; // Each line, i tried different numbers and get Segmentataion error, this numbers was enough
    char allSentences[900] = ""; //Total lines
    
    while(fgets(sentence, 100, encryptedMessagesFile) != NULL)  //Loop in lines of given file
    {
        if (sentence[strlen(sentence) - 1] == '.') // If it ends with period delete it
        {
            sentence[strlen(sentence) - 1] = '\0';
        }
        sentence[0] = toLower(sentence[0]); // Lowering first character
        strcat(allSentences, sentence); // Adding to all sentences array
        strcat(allSentences, " ");
    }

    // Then try to find the most appeared letter, we will then try to find which letter is e by this array order
    char sortedLetters[27]; // Stores the letters by sorted according to their frequency
    int letterFreq[26] = {0}; // To store frequency

    int i;
    for (i = 0; i < strlen(allSentences); i++) // Loop in all characters in the sentence
    {
        if (isalpha(allSentences[i])) // Check if the character is an alphabetic letter
        {
            letterFreq[allSentences[i] - 'a']++; // Increment its frequency
        }
    }

    char letters[26]; // all letters from a to z; abcdef...
    for (i = 0; i < 26; i++) 
    {
        letters[i] = 'a' + i;  // Store the corresponding letter
    }

    int j;
    for (i = 0; i < 26; i++) // Bubble sort according to frequency
    {  
        for (j = 0; j < 26 - i; j++) {
            if (letterFreq[j] < letterFreq[j + 1]) {
                
                int tempFreq = letterFreq[j]; // Swap frequencies
                letterFreq[j] = letterFreq[j + 1];
                letterFreq[j + 1] = tempFreq;

                char tempLetter = letters[j]; // Swap corresponding letters
                letters[j] = letters[j + 1];
                letters[j + 1] = tempLetter;
            }
        }
    }

    for (i = 0; i < 26; i++)  // Copy the sorted letters to sortedLetters array
    {
        sortedLetters[i] = letters[i];
    }
    sortedLetters[26] = '\0';
    // End of the finding most apperead letters by order
    
    int shiftAmount; // Amount of shifts
    int isFound = 0; // Flag to undertand we found the correct shift amount

    for (i = 0; i < 26; i++) // Loop to iterate in sortedLetters array
    {
        shiftAmount = ('e' - sortedLetters[i] + 26) % 26; // Assuming the most appeared letter is e, shift the sentence according to sortedLetters list

        char decryptedSentence[900];
        //Converting all leters by assuming the current letter should be 'e'   
	int k = 0;
        while (allSentences[k] != '\0') // Shifting every letter of allSentences string
        {
            if (allSentences[k] >= 'a' && allSentences[k] <= 'z') // If it is letter
            {
                decryptedSentence[k] = 'a' + (allSentences[k] - 'a' + shiftAmount + 26) % 26; // Shift according to shiftAmount
            }        
            else 
            {
                decryptedSentence[k] = allSentences[k];  // Keep non-letter characters as they are
            }
            k++;
        }
        decryptedSentence[k] = '\0';  // Null-terminate the string
        //End of the convertSentence function

        char copyOfDecryptedSentence[900]; // Copy the sentence for strtok
        strcpy(copyOfDecryptedSentence, decryptedSentence);

        char *word = strtok(copyOfDecryptedSentence, " "); // Use strtok to check if any word in decrypted sentence does have any common letters
        while (word != NULL) // Loop in decrtpyed sentences words
        {
            if (strstr(commonWordsWithE, word) != NULL) // There are occurense of common english word in sentence! Current shift amount is the correct amount
            {
                isFound = 1; // Activate flag
                break; 
            }
            word = strtok(NULL, " ");
        }
        if (isFound) // If flag activated break the second loop
        {
            break;
        }
    }
    if (!isFound) // If no common words found return 0
    {
        shiftAmount = 0;
    }
    fclose(encryptedMessagesFile);
    return shiftAmount; // When the correct shifting found, current shiftAmount indicates correct value
}

void PrintTheMessageWithShifted (const char *fileName, int shiftAmount) 
{
    FILE *encryptedMessagesFile = fopen(fileName, "r"); // File to read encrypted enemy messages
    FILE *decryptedMessagesFile = fopen("decrypted_messages.txt", "a"); // File to write decrypted messages

    fprintf(decryptedMessagesFile, "%s file's output:\n", fileName); // Print file name of encrypted messages 

    char sentence[300]; 
    while(fgets(sentence, 300, encryptedMessagesFile) != NULL)  // Loop in lines of given file
    {        
        char sentenceToPrint[300];
        // Shift the sentence to given shiftAmohnt
	int i = 0;
        while (sentence[i] != '\0') // Loop in sentence letters
        {
            if (sentence[i] >= 'a' && sentence[i] <= 'z') 
            {
                sentenceToPrint[i] =  (sentence[i] - 'a' + shiftAmount + 26) % 26 + 'a'; // Shift the letter
            }
	    else if (sentence[i] >= 'A' && sentence[i] <= 'Z')
            {
                sentenceToPrint[i] = (sentence[i] - 'A' + shiftAmount + 26) % 26 + 'A'; // Shift the letter
            }    
            else 
            {
                sentenceToPrint[i] = sentence[i];  // Keep non-letter characters as they are
            }
            i++;
        }
        sentenceToPrint[i] = '\0';  // Null-terminate the string
        //End of the convertSentenceFunction

        sentenceToPrint[0] = toUpper(sentenceToPrint[0]); // Uppering first character

        fprintf(decryptedMessagesFile, "%s\n", sentenceToPrint); // Print to decryptedMessages file
    }
    fprintf(decryptedMessagesFile, "\n"); // Print new line
  
    fclose(decryptedMessagesFile);
    fclose(encryptedMessagesFile);
}

int main() 
    {
    caeserDecryption(); // Same as other 2, appends everytime when its called

    return 0;
    }
