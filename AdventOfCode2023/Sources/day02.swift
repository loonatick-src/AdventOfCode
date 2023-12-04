//
//  File.swift
//  
//
//  Created by Chaitanya Kumar on 04/12/2023.
//

struct Game {
    // on god I fucking knew that part 2 would be better suited for SoA layout
    typealias Round = (red: Int, green: Int, blue: Int)
    let id: Int
    let rounds: [Round]
    
    static func parseRound<S: StringProtocol>(str: S) -> Round {
        let seq = CharSplitSequence(source: str.trimmingCharacters(in: .whitespacesAndNewlines), delimiter: ",")
        var draws: Round = (red: 0, green: 0, blue: 0)
        for round in seq {
            let count = parseInteriorNum(round)
            if round.contains("red") {
                draws.red += count
            } else if round.contains("blue") {
                draws.blue += count
            } else {
                draws.green += count
            }
        }
        return draws
    }
    
    init<S: StringProtocol>(str: S) {
        let idSplitIter = CharSplitIterator(source: str, delimiter: ":")
        let idString = idSplitIter.next()!
        let idIndex = idString.index(idString.startIndex, offsetBy: 5)
        id = Int(idString[idIndex...])!
        let roundsSplitSeq = CharSplitSequence(source: idSplitIter.next()!, delimiter: ";")
        rounds = roundsSplitSeq.map { Self.parseRound(str: $0) }
    }
}

func parseInteriorNum<S: StringProtocol>(_ line: S) -> Int {
    let startIndex = line.firstIndex(where: { $0.isNumber })!
    let endIndex = line[startIndex...].firstIndex(where: { !$0.isNumber }) ?? line.endIndex
    return Int(line[startIndex..<endIndex])!
}

func solveDay02Part01<S: StringProtocol>(input: S) -> Int {
    let games = CharSplitSequence(source: input, delimiter: "\n").map { Game(str: $0) }
    let reference: Game.Round = (red: 12, green: 13, blue: 14)
    var result = 0
        
    outer: for game in games {
        for round in game.rounds {
            if round.red > reference.red || round.blue > reference.blue || round.green > reference.green {
                continue outer
            }
        }
        result += game.id
    }
    return result
}

func solveDay02Part02<S: StringProtocol>(input: S) -> Int {
    let games = CharSplitSequence(source: input, delimiter: "\n").map { Game(str: $0) }
    return games.map { game in
        let minRed = game.rounds.max { $0.red < $1.red }!.red
        let minBlue = game.rounds.max{ $0.blue < $1.blue }!.blue
        let minGreen = game.rounds.max{ $0.green < $1.green }!.green
        return minRed * minBlue * minGreen
    }.reduce(0, +)
}
