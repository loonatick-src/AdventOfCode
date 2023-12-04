import Foundation

// TODO: an iterator class that takes e.g. a `String` and the `next()` method causes tokenization and parse into a struct
class CharSplitIterator<S: StringProtocol>: IteratorProtocol {
    typealias Element = S.SubSequence
    let delimiter: Character
    let source: S
    var currentIndex: String.Index?
    
    init(source: S, delimiter: Character) {
        self.delimiter = delimiter
        self.source = source
        self.currentIndex = source.startIndex
    }
    
    public func next() -> Element? {
        guard let startIndex = currentIndex else { return nil }
        var endIndex = self.source.endIndex // past the end position
        if let substrEndIndex = source[startIndex...].firstIndex(of: self.delimiter) {
            endIndex = substrEndIndex
            self.currentIndex = self.source.index(after: endIndex)
        } else {
            self.currentIndex = nil
        }
        return self.source[startIndex..<endIndex]
    }
}

class CharSplitSequence<S: StringProtocol>: Sequence {
    typealias Element = CharSplitIterator<S>.Element
    
    let source: S
    let delimiter: Character
    
    init(source: S, delimiter: Character) {
        self.source = source
        self.delimiter = delimiter
    }
    
    public func makeIterator() -> CharSplitIterator<S> {
        return CharSplitIterator(source: self.source, delimiter: self.delimiter)
    }
}

func readAoCInput(pathString: String) throws -> String {
    let url = URL(fileURLWithPath: pathString)
    let data = try Data(contentsOf: url)
    guard let string = String(data: data, encoding: .utf8) else {
        throw NSError(domain: "benchod wtf is domain", code: 500, userInfo: [NSLocalizedDescriptionKey: "Failed to convert data to string"])
    }
    return string.trimmingCharacters(in: .whitespacesAndNewlines)
}

// MARK: Value Semantics and COW
// Let's try to make a binary tree data structure

// this cannot be done because duh
/*
struct BinaryTreeNode<T: Comparable> {
    let value: T
    var left, right: BinaryTreeNode<T>?
}
*/

/// The idea is that T is intended to be a value type, and we wrap this in a class that has
/// reference semantics to get reference semantics for the underlying value type.
/// What can we do with this class?
final class Ref<T> {
    var val: T
    init(_ v: T) {
        self.val = v
    }
}

struct Box<T> {
    var ref: Ref<T>
    init(_ x: T) {
        ref = Ref(x)
    }
    var value: T {
        get { return ref.val }
        set {
            if !isKnownUniquelyReferenced(&ref) {
                ref = Ref(newValue)
                return
            }
            ref.val = newValue
        }
    }
}

struct BinaryTreeNode<T> {
    var val: T
    var left, right: Box<BinaryTreeNode<T>>?
}
