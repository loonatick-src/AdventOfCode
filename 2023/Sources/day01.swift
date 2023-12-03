func solveDay01(inputString: String) -> Int {
    let lines = CharSplitSequence(
      source: inputString.trimmingCharacters(in: .whitespacesAndNewlines),
      delimiter: "\n"
    )
    return lines.map({ extractNum($0) }).reduce(0, +)
}

func unsafeAsciiToNum(_ c: Character) -> Int {
    let asciiZero: Character = "0"
    return Int(c.asciiValue! - asciiZero.asciiValue!)
}

func extractNum<S: StringProtocol>(_ line: S) -> Int {
    let firstIndex = line.firstIndex(where: { $0.isNumber })!
    let firstDigit = unsafeAsciiToNum(line[firstIndex])
    var secondDigit = firstDigit
    let remainingLine = line[line.index(after: firstIndex)...]
    if let secondIndex = remainingLine.lastIndex(where: { $0.isNumber }) {
        secondDigit = unsafeAsciiToNum(remainingLine[secondIndex])
    }
    let result = 10 * firstDigit + secondDigit
    return result
}
//
//final class MatcherState {
//    var value: Int?
//    var transitionStates: [Character: MatcherState]
//    
//    init() {
//        self.value = nil
//        self.transitionStates = [:]
//    }
//    init(value: Int) {
//        self.value = value
//        self.transitionStates = [:]
//    }
//    init(transition: Character) {
//        self.value = nil
//        self.transitionStates = [transition: MatcherState()]
//    }
//    init(transition: Character, nextState: MatcherState) {
//        self.value = nil
//        self.transitionStates = [transition: nextState]
//    }
//
//    func updateTransition(letter: Character, nextState: MatcherState) {
//        self.transitionStates.updateValue(nextState, forKey: letter)
//    }
//}
//
//struct MatcherStateMachine {
//    var initialState: MatcherState
//    mutating func updateInitialState(state: MatcherState) {
//        self.initialState = state
//    }
//    init(on word: String, value: Int) {
//        var currentState = MatcherState(value: value)
//        self.initialState = currentState
//        for letter in word {
//            // TODO: storing the value in every state is wasteful. How can this be improved?
//            let nextState = MatcherState(value: value)
//            currentState.updateTransition(letter: letter, nextState: nextState)
//            currentState = nextState
//        }
//    }
//
//    // init(ensemble: [String: Int]) {
//        
//    // }
//  
//}

func solveDay1Part2(inputString: String) -> Int {
    let lines = CharSplitSequence(
      source: inputString.trimmingCharacters(in: .whitespacesAndNewlines),
      delimiter: "\n"
    )
    var ans = 0
    for line in lines {
        var digits: [Int] = []
        for (i, c) in line.enumerated() {
            if c.isNumber {
                digits.append(unsafeAsciiToNum(c))
                continue
            }
            let subLine = line[line.index(line.startIndex, offsetBy: i)...]
            for (j, num) in ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"].enumerated() {
                if subLine.starts(with: num) {
                    digits.append(j)
                    break
                }
            }
        }
        let result = String(digits[0]) + String(digits[digits.endIndex-1])
        ans += Int(result)!
    }
    return ans
}
