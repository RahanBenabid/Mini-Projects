import Foundation

extension String {
  subscript(_ index: Int) -> Character {
    return self[self.index(self.startIndex, offsetBy: index)]
  }
}

func preprocessing(_ pattern: String) -> [Int] {
  let m: Int = pattern.count
  var i: Int = 1
  var j: Int = 0
  var lps = Array(repeating: 0, count: pattern.count)

  while i < m {
    if pattern[i] == pattern[j] {
      j += 1
      lps[i] = j
      i += 1
    } else {
      if j != 0 {
        j = lps[j - 1]
      } else {
        lps[i] = 0
        i += 1
      }
    }
  }
  return lps
}

// shitty more complex variant
func meilPref(_ x: String) -> [Int] {
  let m = x.count
  var meilPref = Array(repeating: 0, count: m)
  meilPref[0] = -1
  var j = 0
  for i in 1..<m {
    if x[j] == x[i] {
      meilPref[i] = meilPref[j]
    } else {
      meilPref[i] = j
    }
    while j >= 0 && x[i] != x[j] {
      if j == -1 { break }
      j = meilPref[j]
    }
    j += 1
  }
  return meilPref
}

func KMP(text: String, pattern: String) {
  let n: Int = text.count
  let m: Int = pattern.count
  let lps: [Int] = preprocessing(pattern)
  var i = 0
  var j = 0

  while i < n {
    if pattern[j] == text[i] {
      i += 1
      j += 1
    }
    if j == m {
      print("pattern found @ index: \(i-j)")
      j = lps[j - 1]
    } else {
      if i < n && pattern[j] != text[i] {
        if j != 0 {
          j = lps[j - 1]
        } else {
          i += 1
        }
      }
    }
  }
}

let filePath = "./sample.txt"
let pattern: String = "babbaa"
//let pattern: String = "babbaababbb"

let fileURL = URL(fileURLWithPath: filePath)
let text: String = try String(contentsOf: fileURL, encoding: .utf8)

KMP(text: text, pattern: pattern)
