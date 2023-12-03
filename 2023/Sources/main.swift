// The Swift Programming Language
// https://docs.swift.org/swift-book

// let input = try readAoCInput(pathString: "data/day01.txt")
// print(input)

import Foundation

let inputString = try readAoCInput(pathString: "data/day01.txt").trimmingCharacters(in: .whitespacesAndNewlines)

// print(solveDay01(inputString: inputString))

//let matcher = MatcherStateMachine(on: "seven", value: 7)
//print(matcher.initialState.transitionStates["s"]!.transitionStates)

print(solveDay1Part2(inputString: inputString))
