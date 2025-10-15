# Compiler Design Lab - Test Cases

## 1. Lexical Analyzer

<details>
  <summary>Click to expand description of Lexical Analyzer</summary>
  The Lexical Analyzer (Lexer) is responsible for breaking down the input source code into tokens. It identifies keywords, operators, identifiers, and other elements of the language. This section tests the correct tokenization of a given string.
</details>

## 2. Symbol Table

<details>
  <summary>Click to expand description of Symbol Table</summary>
  The Symbol Table stores information about variables, functions, objects, etc., during the compilation process. This section tests the insertion and retrieval operations in the symbol table.
</details>

## 3. Creating a Lexical Analyzer with Lex

<details>
  <summary>Click to expand description of Lexical Analyzer with Lex</summary>
  Lex is a tool used to generate a lexical analyzer. This section tests the ability to write and run Lex specifications to tokenize different types of input.
</details>

## 4. Elimination of Left Recursion

<details>
  <summary>Click to expand description of Elimination of Left Recursion</summary>
  Left recursion in grammar can cause an infinite loop during parsing. This section tests the elimination of left recursion from a given grammar.
</details>

## 5. Finding FIRST and FOLLOW

<details>
  <summary>Click to expand description of Finding FIRST and FOLLOW</summary>
  FIRST and FOLLOW sets are crucial for constructing parsers. This section tests the calculation of the FIRST and FOLLOW sets for different non-terminal symbols.

### Test Cases

  <details>
    <summary>Click to expand Test Case 1</summary>

    Input:

    S -> A B C | c b B | B a
    A -> d a | B C
    B -> g | ε
    C -> b | ε


    Output:

    FIRST(S) = {c, d, g, ε}
    FIRST(A) = {d, g, ε}
    FIRST(B) = {g, ε}
    FIRST(C) = {b, ε}

    FOLLOW(S) = {b, $}
    FOLLOW(A) = {g, ε}
    FOLLOW(B) = {$, a, b, ε}
    FOLLOW(C) = {$, g, ε}

  </details>

  <details>
    <summary>Click to expand Test Case 2</summary>

    Input:

    E -> T E'
    E' -> + T E' | ε
    T -> F T'
    T' -> * F T | ε
    F -> id | ( E )


    Output:

    FIRST(E) = {(, id}
    FIRST(E') = {+, ε}
    FIRST(T) = {(, id}
    FIRST(T') = {*, ε}
    FIRST(F) = {(, id}

    FOLLOW(E) = {$, )}
    FOLLOW(E') = {$, )}
    FOLLOW(F) = {*, ε}
    FOLLOW(T) = {+, ε}
    FOLLOW(T') = {+, ε}

  </details>

  <details>
    <summary>Click to expand Test Case 3</summary>

    Input:

    S -> a B D h
    B -> c C
    C -> b C | ε
    D -> E F
    E -> g | ε
    F -> f | ε


    Output:

    FIRST(S) = {a}
    FIRST(B) = {c}
    FIRST(C) = {b, ε}
    FIRST(D) = {g, ε}
    FIRST(E) = {g, ε}
    FIRST(F) = {f, ε}

    FOLLOW(S) = { $}
    FOLLOW(B) = {g, ε}
    FOLLOW(C) = {g, ε}
    FOLLOW(D) = {h}
    FOLLOW(E) = {f, ε}
    FOLLOW(F) = {h}

  </details>

  <details>
    <summary>Click to expand Test Case 4</summary>

    Input:

    S -> A C B | C b B | B a
    A -> d a | B C
    B -> g | ε
    C -> h | ε


    Output:

    FIRST(S) = {d, g, h, ε}
    FIRST(A) = {d, g, ε}
    FIRST(B) = {g, ε}
    FIRST(C) = {h, ε}

    FOLLOW(S) = { $}
    FOLLOW(A) = {h, ε}
    FOLLOW(B) = { $, a, h, ε}
    FOLLOW(C) = {b, g, h, ε}

  </details>

</details>

## 6. Three Address Code

<details>
  <summary>Click to expand description of Three Address Code</summary>
  Three Address Code (TAC) is an intermediate representation used in compiler design to represent a sequence of operations in a more abstract form. This section tests the generation of TAC for simple expressions.

### Test Cases

  <details>
    <summary>Click to expand Test Case 1</summary>

    Input:

    -(a * b) + (c + d) - (a + b + c + d)


    Output:

    Three Address Code:
    (1) T1 = a * b
    (2) T2 = uminus T1
    (3) T3 = c + d
    (4) T4 = T2 + T3
    (5) T5 = a + b
    (6) T6 = T5 + c
    (7) T7 = T6 + d
    (8) T8 = T4 - T7
    (9) Result = T8

  </details>

  <details>
    <summary>Click to expand Test Case 2</summary>

    Input:

    (-b + sqrt(b * b - 4 * a * c)) / (2 * a)


    Output:

    Three Address Code:
    (1) T1 = uminus b
    (2) T2 = b * b
    (3) T3 = 4 * a
    (4) T4 = T3 * c
    (5) T5 = T2 - T4
    (6) T6 = sqrt T5
    (7) T7 = 2 * a
    (8) T8 = T6 / T7
    (9) Result = T8

  </details>

</details>
