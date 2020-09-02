// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    //Welcoming the Player
    PrintLine(TEXT("Welcome to the ??? ??? Game!"));
    Isograms = GetValidWords(Words);

    //GetValidWords(Words);
    SetupGame();
}


void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bNextWord)
    {
        ClearScreen();
        SetupGame();
    }
    else if (bGameOver)
    {
        ScoreCount = 0;
        Score = 0;
        ClearScreen();
        SetupGame();
    }
    else if (!bGameOver)
    {
        ClearScreen();
        ProcessGuess(PlayerInput);
    }
    
}

void UBullCowCartridge::SetupGame()
{
    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    bNextWord = false;

    PrintLine(TEXT("Score: %i"), Score);
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord); //debug line
    PrintLine(TEXT("You have %i Lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Final Score: %i"), Score);
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::NextWord()
{
    bNextWord = true;
    PrintLine(TEXT("\nPress enter for next word."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ++ScoreCount;
        Score = ScoreCount * 100;
        PrintLine(TEXT("New Score: %i"), Score);
        PrintLine(TEXT("Correct!"));
        NextWord();
        //EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, Try Guessing Again!\nYou have %i lives remaining."), Lives);
        return;
    }

    //Check if Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No Repeating Letters, guess again!"));
    }

    //Remove Life
    PrintLine(TEXT("Lost a Life!"));
    --Lives;

    //Output User Guess

    char GuessArray[16]; //longest isogram in English = 16 letters
    FString correct = "";

    PrintLine(TEXT("\nHere is your guess:"));
    for (int index = 0; index <HiddenWord.Len(); index++)
    {
        if (char(HiddenWord[index]) == char(Guess[index]))
          {
            GuessArray[index] = char(Guess[index]);
          }
        else 
            GuessArray[index] = '_';
    }

    //output in one line
    for (int i = 0; i < HiddenWord.Len(); i++)
    {
        correct = correct + GuessArray[i];
    }
    PrintLine(TEXT("\t %s \n"), *correct); 

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You Have No Lives left!"));
        PrintLine(TEXT("The Hidden Word was %s."), *HiddenWord);
        EndGame();
        return;
    }

    //Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess Again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
