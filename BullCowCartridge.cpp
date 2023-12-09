// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // game starts
{
    Super::BeginPlay();
    Words = GetValidWords(Isograms);
    SetupGame();

    PrintLine(TEXT("The number of possible words is %i."), Words.Num());
    PrintLine(TEXT("The number of valid words is %i"), Words.Num());
}

void UBullCowCartridge::OnInput(const FString& Input) // player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine("WELCOME TO THE GAME!");
    //PrintLine(TEXT("The hidden word is: %s. \nIt is %i characters long."), *HiddenWord, HiddenWord.Len());
    RandomPosition = FMath::RandRange(0, Words.Num() - 1);
    HiddenWord = Words[RandomPosition];

    PrintLine(TEXT("The hidden word is %s and is item %i."), *HiddenWord, RandomPosition);

    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess a %i letter word and press Enter."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives remaining."), Lives);

    //const TCHAR HW[] = TEXT("alert");
    //PrintLine(TEXT("Character 4 of alert is: %c"), HW[3]);

    IsIsogram(HiddenWord);


}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
        //ClearScreen();
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You won! Good for you."));
        EndGame();
        return;
    }
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden Word is %i characters long. Try again!"), HiddenWord.Len());
        PrintLine(TEXT("You still have %i lives left."), Lives);
        return;
    }
    if (IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters! Try again."));
        return;
    }
    --Lives;
    PrintLine(TEXT("You lost a life. \nYou have %i lives remaining."), Lives);
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You are out of lives. Try again later."));
        PrintLine(TEXT("The correct answer was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows."), Bulls, Cows);

    FString BullCowHint;

        for (int32 Index = 0; Index < HiddenWord.Len(); Index++)
        {
            FString GuessCharAsString(1, &Guess[Index]); // Convert single character to FString

            if (Guess[Index] == HiddenWord[Index])
            {
                BullCowHint += "B"; // Add 'B' for correct letter in correct position
            }
            else if (HiddenWord.Contains(GuessCharAsString))
            {
                BullCowHint += "C"; // Add 'C' for correct letter in the wrong position
            }
            else
            {
                BullCowHint += "_"; // Add '_' for incorrect letter
            }
        }

    PrintLine(TEXT("Bull and Cow Hint: %s"), *BullCowHint);

}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {    
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if(Word[Index] == Word[Comparison])
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

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        FString GuessCharAsString(1, &Guess[GuessIndex]); // Convert single character to FString

        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
        }
        else if (HiddenWord.Contains(GuessCharAsString))
        {
            CowCount++;
        }
    }
}