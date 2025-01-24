#include <stdio.h>
#include <string.h>

//Comp 201 Assignment-1 main.c file


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

void yodafication()
    {
    FILE *inputFile = fopen("yodafication.txt", "r"); // Open yodafication.txt file
    FILE *outputFile = fopen("yodafied_sentences.txt", "w"); // Open yodafied_sentence.txt file

    char sentence[100]; // Declare a variable sentence

    while (fgets(sentence, sizeof(sentence), inputFile)) // Loop between lines of yodafication.txt file
           {
           if (strlen(sentence) > 0 && sentence[strlen(sentence) - 1] == '\n')
               {
               sentence[strlen(sentence) - 1] = '\0'; // Add null terminator
               }

           int spaceCount = 0; // For counting the spaces between words
           int secondSpace = -1; // Second space's index is important
           int len = strlen(sentence); // Length of sentence
	   char newSentence[len + 2]; // Yodaficated sentence will be 2 character longer because ", " 

	   sentence[len - 1] = '\0'; // Remove the period
	   sentence[0] = toLower(sentence[0]); // Lower the first letter
           strncpy(newSentence, sentence, len); // Copy original sentence to newSentence

	   int i; // Loop index
           
           for (i = 0; i < len; i++) // Loop into char array to find second space's index
               {
               if (sentence[i] == ' ') // Found a space
                   {
                   spaceCount++;
                   if (spaceCount == 2) // If index is equal to second space's index
                       {
                       secondSpace = i; // Redefine the variable
                       break; // No need to continue
                       }
                   }
               }


           char* firstTwoWords = sentence; // Beginning of the sentence
           char* restWords = sentence + secondSpace + 1; // Pointer starts from the rest of the sentence

           strcpy(newSentence, restWords); // Start adding with rest of the sentence
           strcat(newSentence, ", "); // Add comma
           strncat(newSentence, firstTwoWords, secondSpace); // Add first two words
           strcat(newSentence, "."); // Add period
           newSentence[0] = toUpper(newSentence[0]); // Capitalize first char

           fprintf(outputFile, "%s\n", newSentence); // Print to output file
           }
    fclose(inputFile); // Close yodafication.txt file
    fclose(outputFile); // Close yodafied_sentence.txt file
    }

void replaceWordAndPrintToCensored (char* sentence, char* newWord, char* oldWord)
{
    FILE *outputFile = fopen("censored_sentences.txt", "a"); // Opens censored_sentences file with append mode

    char newSentence[strlen(sentence) - strlen(oldWord) + strlen(newWord) + 1]; // New sentence with proper length
    newSentence[0] = '\0'; // Initialize newSentence as an empty string

    char *pos = sentence;
    char *found = strstr(pos, oldWord);

    strncat(newSentence, pos, found - pos); // Add the first part until the old word
    strcat(newSentence, newWord); // Add the new word
    strcat(newSentence, found + strlen(oldWord)); // Add the remaining part of the sentence

    fprintf(outputFile, "%s", newSentence); // Print to output file
    fclose(outputFile); // Close the file
}

void justPrintToCensored (char* sentence) 
{
    FILE *outputFile = fopen("censored_sentences.txt", "a"); // Open the censored_sentences file with append mode
    fprintf(outputFile, "%s", sentence); // Print to output file
    fclose(outputFile); // Close the file
}

void robotCensorship()
{
    FILE *forbiddenFile = fopen("forbidden.txt", "r"); // Open and read Forbidden words file

    char allForbiddenWords[100]; // Array to store all forbidden words, there are 10 words the length 100 will be enough
    allForbiddenWords[0] = '\0'; // Initialize newSentence as an empty string
    char word[20]; // Temporary array to add words to allForbiddenWords, there are no word longer than 20 characters

    while (fgets(word, 20, forbiddenFile) != NULL)
    {
        word[strcspn(word, "\n")] = '\0'; // Remove the new line character

        strcat(allForbiddenWords, word); // Add word to allForbiddenWords array
        strcat(allForbiddenWords, " "); // Add space between words
    }
    fclose(forbiddenFile); // Close the file

    FILE *censorshipFile = fopen("censorship.txt", "r"); // Open censorship file

    char sentence[100]; // Temporary array to loop in file

    while(fgets(sentence, 100, censorshipFile) != NULL) // Loop in censorship file sentences
    {
        char tempForbiddenWords[strlen(allForbiddenWords)]; // Temporary forbidden words array to use in strtok loop
        strcpy(tempForbiddenWords, allForbiddenWords); // Create a copy for strtok
        char *forbiddenWord = strtok(tempForbiddenWords, " "); // Using strtok to loop in allForbiddenWords array
        int wordIndex = 0; // Index to track the location, for even or odd
        int found = 0; // Flag to check if a forbidden word was found

        while (forbiddenWord != NULL)
        {
            if (strstr(sentence, forbiddenWord) != NULL) // If the word is in the sentence strstr command returns a pointer
            {
                found = 1; // Forbidden word found!

                if (wordIndex % 2 == 0) // The bop and beep decision setted according to sample output
                {
                    replaceWordAndPrintToCensored(sentence, "bop", forbiddenWord); // Replace and print to output file
                }
                else
                {
                    replaceWordAndPrintToCensored(sentence, "beep", forbiddenWord); // Replace and print to output file
                }
            }
            forbiddenWord = strtok(NULL, " "); // Move to the next forbidden word
            wordIndex++;
        }

        if (!found) // If no forbidden words were found, print original sentence
        {
            justPrintToCensored(sentence); // Print to output file
        }
    }
    fclose(censorshipFile); // Close the censorship file
}

// My countWords function i created in InLab3
int countWords(char str1[]) { 
    int word = 0; // Flag
    int wordCount = 0;
    
    int i;
    for (i = 0; i < strlen(str1); i++) 
    {
        if (str1[i] == ' ') // When we hit space
        {
            if (word == 1) { // If we were on a word it means, word ends
                wordCount++; 
                word = 0; // Unflag
            }
        }
        else {
            word = 1; // If it is not space it means we are on a word, and flag 
        }
    }
    if (word == 1) {
        wordCount++; // Checking if the sentence ends without a space
    }
    return wordCount;  
}  

// This function reads the messages and gives a detailed explanation about risk level of words, and flag them
void readMessagesAndFlagThem(char *riskWords, char *fileName) 
{
    FILE *enemyMessagesFile = fopen(fileName, "r"); // Open the given enemyMessages.txt  file
    FILE *riskAssessmentFile = fopen("risk_assessment.txt", "a"); // Open the riskAssessment.txt file

    int totalNumHighRisk = 0; // Total number of high risk level words in the given file
    int totalNumMedRisk = 0; // Total number of medium risk level words in the given file
    int totalNumLowRisk = 0; // Total number of low risk level words in the given file

    char enemyMessages[100] = ""; // Any message is not longer than 100, max is 75

    while(fgets(enemyMessages, 100, enemyMessagesFile)) // Loop in enemyMessages.txt lines
    {
        int numHighRisk = 0; // High risk words counter for each line
        int numMedRisk = 0; // Medium risk words counter for each line

	enemyMessages[0] = toLower(enemyMessages[0]); // Lower the first letter of sentence

        char tempRiskWords[strlen(riskWords)]; // Create a temporary riskWords
        strcpy(tempRiskWords, riskWords); // Copy
        char *word = strtok(tempRiskWords, " "); // Since strtok function modifies the original function it need to be copied

        while (word != NULL) // Loop in copied risk words array
        {
	    // Since i need to remove last 2 digits of word, i need a copied temporary word
            char tempWord[strlen(word)]; // Create a temporary word variable with same lenght as original word
            strcpy(tempWord, word); // Copy the original word
            tempWord[strlen(tempWord)-1] = '\0';
            tempWord[strlen(tempWord)-2] = '\0'; // Deleting last 2 characters

            if (strstr(enemyMessages, tempWord) != NULL) // If a risk word occurs in the sentence
            {
                char lastDigit = word[strlen(word)-1]; // Get the last character which determines it risk level

                if (lastDigit == 'h') // If h
                {
                    numHighRisk++; // Increment
                }
                else if (lastDigit == 'm') // If m
                {
                    numMedRisk++; // Increment
                }
            }
            word = strtok(NULL, " "); // Step to next word
        }
        int totalWords = countWords(enemyMessages); // calculate total words to calculate low risk level words
        totalNumLowRisk += totalWords - numHighRisk - numMedRisk; // Calculate low risk level words
        totalNumHighRisk += numHighRisk; // Add high risk words to total high risk words
        totalNumMedRisk += numMedRisk; // Add medium risk words to total medium risk words
    }
    fprintf(riskAssessmentFile, "Message: %s\n", fileName); // Print the message name to txt file
    fprintf(riskAssessmentFile, "High-risk words: %d\n", totalNumHighRisk); // Print the high risk words number to txt file
    fprintf(riskAssessmentFile, "Medium-risk words: %d\n", totalNumMedRisk); // Print the medium risk words number to txt file
    fprintf(riskAssessmentFile, "Low-risk words: %d\n", totalNumLowRisk); // Print the low risk words number to txt file

    if ((5*(totalNumHighRisk) + 3*(totalNumMedRisk)) > totalNumLowRisk) // Check, if the condition you give is valid
    {
        fprintf(riskAssessmentFile, "Status: Flagged\n"); // Make status flagged if valid
    }
    else 
    {
        fprintf(riskAssessmentFile, "Status: Not Flagged\n"); // Make status not flagged if not valid
    }
    fprintf(riskAssessmentFile, "\n"); // Print new line

    fclose(enemyMessagesFile); // Close the given enemyMessages file
    fclose(riskAssessmentFile); // Close the riskAssessment file
}

void riskAssessment() 
{
    char riskWords[180] = ""; // There are 18 words in risk_words.txt file, so 180 will be enough

    char word[20]; // Temporary word char array to get file words

    FILE *riskWordsFile = fopen("risk_words.txt", "r"); // Open the risk_words.txt file

    while(fgets(word, 20, riskWordsFile) != NULL) // Loop in risk_words.txt file's lines
    {
        if (word[strlen(word)-1] == '\n') // If the last digit before null terminator is \n
        {
           word[strlen(word)-1] = '\0'; // Remove new line character 
        }

        strcat(riskWords, word); // Add to riskWords array
        strcat(riskWords, " "); // Add a space between words
    }
    fclose(riskWordsFile); // Close the file

    readMessagesAndFlagThem(riskWords, "enemy_messages/message1_encrypted.txt"); // Analyze the first message, and print the output
    readMessagesAndFlagThem(riskWords, "enemy_messages/message2_encrypted.txt"); // Analyze the second message, and print the output
    readMessagesAndFlagThem(riskWords, "enemy_messages/message3_encrypted.txt"); // Analyze the third message, and print the output
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
    yodafication();

    robotCensorship(); // Each time it is called, appends the output to the censored_sentences.txt file, so try not to call it multiple times

    riskAssessment(); // This also appends to end of the file, so be careful about calling it multiple times

    caeserDecryption(); // Same as other 2, appends everytime when its called

    return 0;
    }
