# C++ Class 與 Operator Overloading：從零理解

> 適合對象：已學過 `struct`、函式、基本指標和參考（reference），但還不熟悉 `class` 與運算子多載的學生。
> 閱讀方式：每節先理解「想解決什麼問題」，再看程式碼。各節會逐步替同一個 `Point` 類別增加功能。
> 內容範圍：依原講義的七個主題安排，補上動機、逐步推導、執行過程與常見疑問；**每節後面有一關練習，並預留作答區與可展開的完整解答**，第 9 節另附總整合程式。
> 練習使用說明：原練習檔用 `Vec2` 命名，這裡為與正文一致，統一改成 `Point`；練習要求及測試重點保留。建議先在預留區思考與寫程式，再展開參考答案核對。

---

## 先建立一個觀念：物件不神祕，就是「一份資料」

先想像平面上的一個點，它需要兩個整數：`x` 和 `y`。

```cpp
struct Point {
    int x;
    int y;
};

Point a = {2, 3};
```

這裡的 `a` 是一個 `Point` 物件，裡面存著 `x = 2`、`y = 3`。以前我們用 `struct` 把資料放在一起，這個想法沒有錯。

接下來的問題是：**我們能不能把「這份資料允許怎麼被使用」也一起寫進型別裡？**

例如，一個點除了儲存座標，可能還需要：

- 讀取座標。
- 移動座標。
- 比較兩個點是否相同。
- 把兩個點的座標相加。

C++ 的 `class` 和 `struct` 都做得到這些事。這份教材用 `class` 來練習「把資料與操作方式放在同一個型別中」。

## 1. Class、存取權限與 Constructor

### 1.1 為什麼需要 `private` 和 `public`？

考慮一個銀行帳戶。如果餘額是公開的：

```cpp
struct BankAccount {
    int balance;
};

BankAccount a{100};
a.balance = -999;
```

程式可以直接把餘額改成負數，完全沒有經過檢查。為了控制外部程式如何修改資料，我們可以把餘額設成 `private`，再提供 `public` 函式。

```cpp
class BankAccount {
private:
    int balance = 0;

public:
    bool deposit(int amount);
};
```

讀法：

- `private`：只有這個類別本身（及被允許的特定程式碼）能直接存取。外部不能寫 `a.balance = -999`。
- `public`：提供給外部使用的介面，例如 `a.deposit(50)`。
- `balance = 0`：建立物件時，初始餘額是 0。

`deposit` 可以定義成：

```cpp
bool BankAccount::deposit(int amount) {
    if (amount <= 0) return false;
    balance += amount;
    return true;
}
```

這裡 `BankAccount::` 是在說：**這個函式屬於 `BankAccount`**，雖然實作寫在類別外面。

假設原本餘額是 0：呼叫 `deposit(50)` 後變成 50、回傳 `true`；再呼叫 `deposit(-3)` 時會回傳 `false`，餘額維持 50。

### 1.2 既然 `struct` 也能做，為什麼還要 `class`？

這點很容易誤解：**在 C++ 中，`struct` 也能有 `private`、建構子、成員函式、繼承與運算子多載。**

兩者的語言差異主要在「沒有特別寫出權限時」的預設值：

|                  | `struct` | `class`   |
| ---------------- | ---------- | ----------- |
| 成員預設存取權限 | `public` | `private` |
| 繼承時的預設權限 | `public` | `private` |

平常的使用習慣是：單純裝資料的型別常寫成 `struct`；希望隱藏內部狀態、只開放特定操作的型別常寫成 `class`。**這是慣例，不是功能上的硬性分工。**

### 1.3 Constructor（建構子）到底做什麼？

如果我們希望每個 `Point` 在建立時就有一組座標，可以寫：

```cpp
class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}
};
```

看到 `Point(int initialX, int initialY)`，它像函式卻沒有回傳型別，因為它是 **Constructor（建構子）**。

```cpp
Point a(2, 3);
```

執行這行時：

1. 建立一個 `Point` 物件 `a`。
2. 把 `2` 傳給 `initialX`，把 `3` 傳給 `initialY`。
3. `: x(initialX), y(initialY)` 把成員 `x` 初始化為 2、`y` 初始化為 3。
4. `{}` 是建構子主體；這個例子不必另外執行動作。

**特別注意：**冒號後面是 *member initializer list*（成員初始化串列）。它不是在主體內先建立成員再指定新值，而是直接初始化成員。現階段先記住它的用途就好。

### 1.4 `private` 的資料，之後要怎麼讀？

可以提供公開的 getter：

```cpp
int getX() const { return x; }
int getY() const { return y; }
```

這兩行會放在 `Point` 類別的 `public:` 區塊中。於是可以寫：

```cpp
Point a(2, 3);
std::cout << a.getX();  // 2
// std::cout << a.x;    // 錯誤：x 是 private
```

注意：`private` 並不是保證資料永遠不可能改變，而是**將「能如何存取與修改資料」交給類別設計者決定**。

### ✍️ 本節練習｜第 1 關：自己從零寫出 `Point`

**題目**

請宣告一個 `class Point`，完成以下要求：

1. `x`、`y` 是 `private` 的 `int`；建立物件時必須給定兩個座標。
2. 提供 `getX()`、`getY()`，分別回傳座標；提供 `print()`，以 `(x, y)` 格式輸出，不要自行換行。
3. 上述三個讀取函式都必須能讓 `const Point` 呼叫。
4. 用 `const Point fixed(3, 4);` 測試；再試著在 `main()` 裡寫 `v.x = 8;`，觀察為什麼不能直接存取 `private` 欄位（測試完請註解掉）。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};

int main() {
    const Point fixed(3, 4);
    std::cout << fixed.getX() << '\n';
    std::cout << fixed.getY() << '\n';
    fixed.print();
    std::cout << '\n';

    Point v(3, 4);
    // v.x = 8;  // 解除註解會編譯失敗：x 是 private
    v.print();
    std::cout << '\n';
}
```

**預期輸出**

```text
3
4
(3, 4)
(3, 4)
```

**理解重點：**`print()` 只是把資料印出來，不修改座標，所以也應標上 `const`。而 `private` 的意思不是完全不能讀，而是不能由類別外直接讀寫；我們可以透過 `public` 的方法提供受控的存取方式。

</details>

---

## 2. Member Function 與 `const`

### 2.1 成員函式操作的是哪一個物件？

假設替 `Point` 加上一個移動座標的函式：

```cpp
void move(int dx, int dy) {
    x += dx;
    y += dy;
}
```

這段放在 `Point` 的 `public:` 區塊。當我們寫：

```cpp
Point a(2, 3);
a.move(4, -1);
```

`move` 操作的就是呼叫它的 `a`：原本 `(2, 3)`，移動後是 `(6, 2)`。`a` 與其他 `Point` 物件各自有自己的 `x`、`y`。

### 2.2 為什麼 `const` 有時在參數前、有時在函式後？

先看函式**後面**的 `const`：

```cpp
int getX() const { return x; }
```

這裡的意思是：**呼叫 `getX()` 不應透過這個成員函式修改物件的狀態。**

因此，下面的寫法可以成立：

```cpp
const Point fixed(2, 3);
std::cout << fixed.getX();  // 可以
// fixed.move(1, 1);       // 不可以：move 不是 const 成員函式
```

再看參數中的 `const Point&`：

```cpp
void printPoint(const Point& p) {
    std::cout << "(" << p.getX() << ", "
              << p.getY() << ")";
}
```

它其實包含兩件事：

- `&`：以**參考**接收呼叫端的物件，不需要另外複製整個 `Point`。
- `const`：這個函式**不能透過參數 `p` 修改物件**。

想像 `p` 是物件的一個「唯讀別名」；它仍然指向原本那個物件，而不是建立一個新的。

**比較一下兩種 `const`：**

| 寫法                                | 保護誰？                           |
| ----------------------------------- | ---------------------------------- |
| `void printPoint(const Point& p)` | 不准透過參數 `p` 修改傳入的物件  |
| `int getX() const`                | 不准這個成員函式修改它所操作的物件 |

後面寫 Operator Overloading 時，這兩種寫法會一起出現。要特別能分辨它們。

### ✍️ 本節練習｜第 2 關：分辨「會修改」與「只讀取」的成員函式

**題目**

延續上一關的 `Point`，新增：

1. `move(int dx, int dy)`：把 `x` 加上 `dx`、`y` 加上 `dy`。
2. `lengthSquared()`：回傳 `x*x + y*y`，但不修改座標。
3. 測試 `Point v(3, 4); v.move(-1, 2);`，檢查座標及 `lengthSquared()`。
4. 驗證 `const Point fixed(3, 4);` 能呼叫 `lengthSquared()`，卻不能呼叫 `move()`。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }
};

int main() {
    Point v(3, 4);
    v.move(-1, 2);
    v.print();
    std::cout << '\n';
    std::cout << v.lengthSquared() << '\n';

    const Point fixed(3, 4);
    std::cout << fixed.lengthSquared() << '\n';
    // fixed.move(1, 1);  // 解除註解會編譯失敗
}
```

**預期輸出**

```text
(2, 6)
40
25
```

**理解重點：**`move()` 會修改呼叫它的物件，所以不加函式尾端的 `const`。`lengthSquared()` 只計算並回傳數字，所以應加 `const`。注意：名稱中的 *Squared* 是「平方」，不是開根號後的距離。

</details>

---

## 3. `==` 與 `!=`：為什麼要 Operator Overloading？

### 3.1 一個普通的 `+` 或 `==`，其實已經有人替你定義好了

C++ 知道：

```cpp
2 + 3       // 5
2 == 3      // false
```

因為語言本身知道整數的加法與比較應該怎麼做。但它並不知道你自己設計的 `Point`，應該根據什麼條件判定「兩個點相等」。

我們可以自己決定：**兩個 `Point` 的 `x` 相同、`y` 也相同，就算相等。**

Operator Overloading（運算子多載）就是讓既有的運算子，也能用在自訂型別上，並指定它的行為。

### 3.2 寫出 `operator==`

替 `Point` 加上：

```cpp
bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
}
```

別急著背。先看看這樣用會發生什麼事：

```cpp
Point a(2, 3);
Point b(2, 3);
Point c(2, 5);

std::cout << (a == b);  // 1（true）
std::cout << (a == c);  // 0（false）
```

在這個例子裡，`a == b` 可以理解成：

```cpp
a.operator==(b);
```

因此：

1. 左邊的 `a` 是**呼叫這個成員函式的物件**。函式裡的 `x`、`y` 指的就是 `a.x`、`a.y`。
2. 右邊的 `b` 傳進參數 `other`。所以 `other.x`、`other.y` 指的是 `b` 的座標。
3. 比較兩個 `x` 是否相等、兩個 `y` 是否相等。
4. 只有兩個條件都成立（`&&`），才回傳 `true`。

這行宣告的三個重點是：

```cpp
bool operator==(const Point& other) const
```

- `bool`：因為判斷相等的結果只有 `true` 或 `false`。
- `const Point& other`：不複製右邊的物件，也不修改它。
- 最後的 `const`：比較時不修改左邊的物件。

### 3.3 `!=` 不需要從頭寫一次

「不相等」就是「相等」的相反，因此：

```cpp
bool operator!=(const Point& other) const {
    return !(*this == other);
}
```

初學者常被 `*this` 卡住，這裡拆開說：

- `this`：在非 `static` 成員函式裡，指向**目前呼叫函式的物件**的指標。
- `*this`：把這個指標解參考，取得**目前這個物件本身**。
- `*this == other`：利用剛剛定義的 `==` 比較左右兩個 `Point`。
- `!`：把布林值反過來；`true` 變 `false`，`false` 變 `true`。

所以 `a != b` 的實際問題是：「`a == b` 的結果是不是假的？」

你也可以直接比較兩組座標來實作 `!=`，但重用 `==` 的好處是**只需要維護一份『什麼叫相等』的規則**。

### ✍️ 本節練習｜第 3 關：自己定義 `==` 和 `!=`

**題目**

延續前面的 `Point`：

1. 只有當兩個點的 `x` 與 `y` **都相同**時，`a == b` 才是 `true`。
2. `a != b` 要重用 `==`，不要把座標比較邏輯再寫一次。
3. 比較不應修改任一物件；也要能比較 `const Point`。
4. 測試相同座標、只有 `x` 不同、只有 `y` 不同等情況。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

int main() {
    const Point a(2, 3);
    const Point b(2, 3);
    const Point c(2, 4);  // 只有 y 不同
    const Point d(9, 3);  // 只有 x 不同

    std::cout << std::boolalpha;
    std::cout << (a == b) << '\n';
    std::cout << (a == c) << '\n';
    std::cout << (a != c) << '\n';
    std::cout << (a == d) << '\n';
    std::cout << (a != d) << '\n';
}
```

**預期輸出**

```text
true
false
true
false
true
```

**理解重點：**`a == b` 可看成 `a.operator==(b)`；`other` 是右邊的 `b`。`*this` 則代表左邊的 `a`。`const Point& other` 保護右邊參數，函式末端的 `const` 保護左邊的呼叫者。

</details>

---

## 4. `+=`：修改原本的物件

### 4.1 `a += b` 到底應該改變誰？

對整數來說：

```cpp
int a = 2, b = 3;
a += b;  // a 變成 5；b 還是 3
```

我們希望 `Point` 也能這麼做。例如 `a = (2, 3)`、`b = (4, 5)`：

```text
操作前：a = (2, 3)，b = (4, 5)
a += b
操作後：a = (6, 8)，b = (4, 5)
```

因此 `+=` 的責任很明確：**修改左邊物件，保留右邊物件**。

### 4.2 實作 `operator+=`

在 `Point` 類別裡加入：

```cpp
Point& operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
}
```

前兩行好懂：直接修改這個物件的 `x` 和 `y`。

真正要注意的是：**為什麼回傳 `Point&`，而不是 `Point` 或 `void`？**

因為 `*this` 是目前的物件，而 `Point&` 表示「回傳目前物件的參考」。這讓運算式的結果仍然指向左邊的同一個物件。

```cpp
Point a(1, 2), b(3, 4), c(5, 6);
(a += b) += c;
```

執行順序：

1. `a += b`：`a` 從 `(1, 2)` 變成 `(4, 6)`。
2. 第一個 `+=` 回傳的還是 `a` 本身。
3. 接著再 `+= c`：`a` 變成 `(9, 12)`。

如果 `operator+=` 回傳 `void`，就不能把第一個 `a += b` 的結果繼續拿來做 `+= c`。

**一個邊界情況：**`a += a` 也是合法的。在這個座標例子中，`a` 會變成原本的兩倍。

### ✍️ 本節練習｜第 4 關：實作會修改左邊的 `+=`

**題目**

1. 實作 `a += b`：讓 `a` 的 `x`、`y` 分別增加 `b` 對應的座標，但 **`b` 不變**。
2. 讓 `(a += b) += c` 也能使用，因此應回傳左邊**原本的物件**。
3. 測試 `a += a`：同一個物件同時在運算子的左右兩邊，結果應讓兩個座標都加倍。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

int main() {
    Point a(1, 2), b(3, 4), c(5, 6);

    a += b;
    a.print(); std::cout << '\n'; // (4, 6)
    b.print(); std::cout << '\n'; // (3, 4)

    a += a;
    a.print(); std::cout << '\n'; // (8, 12)

    Point d(1, 2);
    (d += b) += c;
    d.print(); std::cout << '\n'; // (9, 12)
}
```

**預期輸出**

```text
(4, 6)
(3, 4)
(8, 12)
(9, 12)
```

**理解重點：**`Point&` 是參考回傳，所以第一個 `+=` 的結果仍是原本的左邊物件。`a += a` 在此實作中也正確，因為左右都指向同一個 `a`，只是把每個座標加上自身。

</details>

---

## 5. `+`：產生新結果，但不要修改原物件

這一節是最容易與 `+=` 搞混的地方。

### 5.1 先比較兩種行為

假設 `a = (2, 3)`、`b = (4, 5)`。

| 寫法          | 操作結束後的 `a` | 得到的結果          |
| ------------- | ------------------ | ------------------- |
| `a += b`    | `(6, 8)`         | 左邊的 `a` 本身   |
| `c = a + b` | `(2, 3)`（不變） | 新的 `c = (6, 8)` |

可以把它記成：

- `+=`：「**把右邊加到我身上**」。
- `+`：「**算出我們的和，但不要動我們**」。

### 5.2 利用已經寫好的 `+=` 實作 `+`

在 `Point` 類別加入：

```cpp
Point operator+(const Point& other) const {
    Point result(*this);
    result += other;
    return result;
}
```

逐行想：

1. `Point result(*this);`：用目前物件建立一份新的 `Point`（此處使用複製建構子）。
2. `result += other;`：只修改這個新物件；不動原本的左、右物件。
3. `return result;`：把計算好的 `Point` **以值回傳**。

假設：

```cpp
Point a(2, 3), b(4, 5);
Point c = a + b;
```

操作後 `a` 仍是 `(2, 3)`、`b` 仍是 `(4, 5)`，`c` 才是 `(6, 8)`。

### 5.3 為什麼 `+` 回傳 `Point`，而不是 `Point&`？

因為這裡的 `result` 是函式內建立的區域物件。函式結束後，這個區域物件的生命週期就結束了。

如果回傳指向 `result` 的參考，外面就會拿到**已經失效的參考**，非常危險。

所以：

- `+=` 修改既有的左邊物件 → 通常回傳 `Point&`。
- `+` 建立新的計算結果 → 通常回傳 `Point`（值）。

> 補充：現代 C++ 編譯器通常能省略不必要的複製或移動，這不改變「這裡應以值回傳」的設計原則。

### ✍️ 本節練習｜第 5 關：實作不修改原物件的 `+`

**題目**

1. 讓 `a + b` 建立一個**新的** `Point`，不修改原本的 `a`、`b`。
2. 重用上一關寫好的 `+=`，不要再把座標加法邏輯複製一遍。
3. 測試 `a + b + b`、`a + b + c` 等連續加法。
4. 解釋為什麼 `operator+` 應回傳 `Point`（值），而不是 `Point&`（對區域變數的參考）。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator+(const Point& other) const {
        Point result(*this); // 先複製左邊的物件
        result += other;     // 只修改複製品
        return result;       // 以值回傳新結果
    }
};

int main() {
    Point a(1, 2), b(3, 4), c(5, 6);
    Point sum = a + b;
    Point twiceB = a + b + b;
    Point all = a + b + c;

    sum.print();    std::cout << '\n';
    twiceB.print(); std::cout << '\n';
    all.print();    std::cout << '\n';
    a.print();      std::cout << '\n';
    b.print();      std::cout << '\n';
}
```

**預期輸出**

```text
(4, 6)
(7, 10)
(9, 12)
(1, 2)
(3, 4)
```

**理解重點：**`+=` 改左邊，`+` 創造新結果。`result` 是 `operator+` 裡的區域物件；如果把它以 `Point&` 回傳，函式結束就會留下無效參考。因此應使用 `Point` 值回傳。

</details>

---

## 6. `[]`：讓 Point 也能像陣列一樣索引

### 6.1 為什麼想讓 `Point` 使用 `[]`？

假設我們約定：

- `p[0]` 代表 `p` 的 `x`。
- `p[1]` 代表 `p` 的 `y`。

那我們就可以寫出：

```cpp
Point p(2, 3);
std::cout << p[0];  // 2
p[1] = 9;          // 把 y 改成 9
```

這不是說 `Point` 自動變成陣列，而是**我們為 `Point` 定義了 `[]` 的意義**。

### 6.2 為什麼「回傳值」和「回傳參考」會影響能不能指定？

先不要看 `Point`，只看普通整數：

```cpp
int x = 2;
int copy = x;
copy = 9;       // x 還是 2

int& alias = x;
alias = 9;      // x 變成 9
```

`copy` 是另一份整數；`alias` 則是原本 `x` 的參考。因此，如果希望 `p[1] = 9` 真正修改 `p` 的資料，`operator[]` 就需要能回傳對內部成員的參考。

### 6.3 可以修改的版本

在 `Point` 裡加入：

```cpp
int& operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    std::exit(1);
}
```

因為回傳型別是 `int&`，所以 `p[0]` 代表內部的 `x` 本身，不只是 `x` 的數值副本。這就是 `p[0] = 7` 能成立的原因。

但合法索引只有 0、1。這裡沿用原講義的示範：非法索引呼叫 `std::exit(1)` 結束程式；使用時要引入 `<cstdlib>`。實務上也可以設計成丟出例外等其他處理方式。

### 6.4 如果物件是 `const` 呢？

這時我們不希望 `p[0] = 7` 成功，因此再加上一個唯讀版本：

```cpp
int operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    std::exit(1);
}
```

這兩個 `operator[]` 可以同時存在，因為一個是 `const` 成員函式，另一個不是：

```cpp
Point a(2, 3);
a[0] = 7;                // 使用可修改版本

const Point b(4, 5);
std::cout << b[0];       // 使用唯讀版本
// b[0] = 7;             // 錯誤：不能指定
```

**這節要帶走的關鍵：**當運算式需要放在 `=` 左邊、代表「原物件裡某個可修改的位置」時，回傳參考通常很重要。

### ✍️ 本節練習｜第 6 關：讓 `[]` 可以讀，也可以修改

**題目**

1. 約定 `p[0]` 對應 `x`、`p[1]` 對應 `y`。
2. 一般物件可以寫 `p[0] = 9`；`const Point` 可以用 `fixed[1]` **讀取**，但不能透過它修改座標。
3. 若索引不是 `0` 或 `1`，要印出錯誤訊息，並以**非零狀態碼**結束程式。
4. 先測試合法存取，再另外執行 `p[2]` 測試非法索引（因為程式會立刻結束）。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <cstdlib>
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator+(const Point& other) const {
        Point result(*this);
        result += other;
        return result;
    }

    int& operator[](int index) { // 一般物件：可修改
        if (index == 0) return x;
        if (index == 1) return y;
        std::cerr << "無效索引：" << index << '\n';
        std::exit(EXIT_FAILURE);
    }

    int operator[](int index) const { // const 物件：僅讀取
        if (index == 0) return x;
        if (index == 1) return y;
        std::cerr << "無效索引：" << index << '\n';
        std::exit(EXIT_FAILURE);
    }
};

int main() {
    Point p(1, 2);
    p[0] = 9;
    p.print();
    std::cout << '\n';

    const Point fixed(3, 4);
    std::cout << fixed[1] << '\n';

    // fixed[1] = 9; // 解除註解會編譯失敗
    // std::cout << p[2]; // 單獨解除註解：印出錯誤並結束
}
```

**預期輸出（維持註解時）**

```text
(9, 2)
4
```

**非法索引的額外測試：**把 `std::cout << p[2];` 那行解除註解，重新編譯執行，將出現 `無效索引：2`，且程式以非零狀態結束；不應與正常路徑一起測試。

**理解重點：**一般版本回傳 `int&`，使 `p[0]` 成為可以指定的左值；`const` 版本回傳的是 `int` 值，不能拿來修改內部資料。兩個函式只差最後的 `const`，但面對的物件不同。

</details>

---

## 7. 成員運算子與非成員運算子

### 7.1 為什麼 `p + 3` 可以，`3 + p` 卻不一定可以？

對我們在類別內定義的成員運算子來說：

```cpp
p + 3
```

可以理解成：

```cpp
p.operator+(3)
```

左邊 `p` 是 `Point`，所以它可以呼叫 `Point` 的成員函式。

但：

```cpp
3 + p
```

左邊是 `int`，它不是 `Point`，自然不能用 `Point` 的成員函式形式來處理左邊的 `3`。

### 7.2 先讓 `p + 3` 成立

這裡約定「一個 Point 加上一個整數」的意義是：**兩個座標都加上該整數**。

```cpp
Point operator+(int amount) const {
    return Point(x + amount, y + amount);
}
```

因此 `(2, 3) + 3` 會產生 `(5, 6)`。

### 7.3 再讓 `3 + p` 成立

這次把運算子寫成**類別外的一般函式**：

```cpp
Point operator+(int amount, const Point& p) {
    return p + amount;
}
```

這個函式接收**兩個參數**：左邊整數和右邊 `Point`。它直接重用剛剛寫好的 `p + amount`，不需要存取私有成員。

```cpp
Point p(2, 3);
Point a = p + 3;  // (5, 6)
Point b = 3 + p;  // (5, 6)
```

**常見誤會：**「類別外的運算子一定要加 `friend`」並不正確。像這裡只呼叫公開函式，就不需要 `friend`。`friend` 是另一種明確授權存取權限的機制，不是非成員運算子的必要條件。

### ✍️ 本節練習｜第 7 關：支援 `p + 3` 與 `3 + p`

**題目**

1. 定義「點加整數」的意義：把 `x` 和 `y` **都加上該整數**，產生新點。
2. 讓 `p + 3` 可以使用，再讓 `3 + p` 也可以使用。
3. 兩種運算都**不能修改原本的 `p`**。
4. 比較：哪一種寫成 `Point` 的成員運算子？哪一種要寫成非成員運算子？非成員函式不應直接讀寫 `private` 成員。

**📝 作答區（請先自行完成程式，再往下看答案）**

```cpp
// 在這裡寫下自己的 Point 類別與 main()。














// 寫完再展開下方的參考答案。
```

<details>
<summary><strong>🔍 點此展開參考答案（完整程式、執行結果與解釋）</strong></summary>

**參考答案（完整程式，可獨立編譯）**

```cpp
#include <cstdlib>
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    int lengthSquared() const {
        return x * x + y * y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator+(const Point& other) const {
        Point result(*this);
        result += other;
        return result;
    }

    Point operator+(int amount) const { // p + 3
        return Point(x + amount, y + amount);
    }

    int& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        std::cerr << "無效索引：" << index << '\n';
        std::exit(EXIT_FAILURE);
    }

    int operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        std::cerr << "無效索引：" << index << '\n';
        std::exit(EXIT_FAILURE);
    }
};

// 非成員函式：左邊是 int，不是 Point
Point operator+(int amount, const Point& p) {
    return p + amount;  // 重用公開的成員 operator+
}

int main() {
    const Point p(2, 5);
    Point a = p + 3;
    Point b = 3 + p;

    a.print(); std::cout << '\n';
    b.print(); std::cout << '\n';
    p.print(); std::cout << '\n';
}
```

**預期輸出**

```text
(5, 8)
(5, 8)
(2, 5)
```

**理解重點：**`p + 3` 可以呼叫 `p.operator+(3)`；`3 + p` 的左邊是 `int`，不能呼叫 `Point` 的成員函式，所以要用非成員版本 `operator+(3, p)`。非成員版本只需要呼叫 `p + amount`，**不需要 `friend`**。

</details>

---

## 8. 最容易考、也最容易混淆的對照表

| 運算          | 主要意義              | 修改左邊？ | 典型回傳型別            |
| ------------- | --------------------- | ---------- | ----------------------- |
| `a == b`    | 比較兩者是否相同      | 不會       | `bool`                |
| `a != b`    | 判斷兩者是否不同      | 不會       | `bool`                |
| `a += b`    | 把 `b` 加到 `a`   | 會         | `Point&`              |
| `a + b`     | 算出新結果            | 不會       | `Point`               |
| `a[0] = 9`  | 修改 `a` 裡某個成員 | 會         | `int&`（可修改版）    |
| `constA[0]` | 讀取唯讀物件的成員    | 不會       | `int`（此講義的版本） |

### 再用三句話區分

1. `const Point& other`：右邊的輸入物件不能透過 `other` 被修改。
2. `operator+(...) const`：左邊呼叫運算子的物件不能被這個成員函式修改。
3. `return *this`：回傳目前的物件；若回傳型別為 `Point&`，就是回傳目前物件的參考。

### C++ 運算子多載的限制

- 不能創造原本不存在的新運算子，例如 `**`。
- 不能改變既有運算子的優先順序和運算元數量。
- 不能重新定義兩個純內建型別之間的運算，例如讓 `int + int` 改成相減；至少一個運算元要是自訂型別或符合語言允許的類別／列舉型別。

這是補充的語言規則，原講義主要聚焦在如何定義這些運算子。

---

## 9. 把所有概念接成一支能執行的程式

前面的每一節都只是「新增到 `Point` 的某一部分」。以下把它們合併成完整程式，能直接放進 `main.cpp` 編譯執行。

```cpp
#include <cstdlib>
#include <iostream>

class Point {
private:
    int x, y;

public:
    Point(int initialX, int initialY)
        : x(initialX), y(initialY) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator+(const Point& other) const {
        Point result(*this);
        result += other;
        return result;
    }

    Point operator+(int amount) const {
        return Point(x + amount, y + amount);
    }

    int& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        std::exit(1);
    }

    int operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        std::exit(1);
    }
};

Point operator+(int amount, const Point& p) {
    return p + amount;
}

void printPoint(const Point& p) {
    std::cout << "(" << p.getX() << ", "
              << p.getY() << ")";
}

int main() {
    Point a(2, 3), b(4, 5);

    std::cout << std::boolalpha << (a == b) << '\n';

    Point c = a + b;
    printPoint(c);
    std::cout << '\n';

    a += b;
    printPoint(a);
    std::cout << '\n';

    c[0] = 9;
    printPoint(c);
    std::cout << '\n';

    const Point fixed(7, 8);
    std::cout << fixed[1] << '\n';

    Point d = 3 + fixed;
    printPoint(d);
    std::cout << '\n';
}
```

編譯指令（假設檔名是 `main.cpp`）：

```bash
g++ -std=c++17 main.cpp -o main
./main
```

預期輸出：

```text
false
(6, 8)
(6, 8)
(9, 8)
8
(10, 11)
```

建議你實際執行後，把 `a += b` 改成 `a = a + b`，再觀察兩種寫法的行為和回傳結果有何不同。

---

## 一頁記住整章

**Class：**把資料與操作資料的函式放在一起；`class` 預設 `private`，`struct` 預設 `public`。**Constructor：**建立物件時初始化其成員。**`const` 成員函式：**承諾不透過該成員函式修改物件的狀態。**Operator Overloading：**替自訂型別定義既有運算子的行為。**`==`：**比較並回傳 `bool`。**`+=`：**修改原物件，常回傳原物件的參考。**`+`：**產生新物件，通常以值回傳。**`[]`：**要支援 `p[0] = 9`，需提供可修改的參考。**成員與非成員運算子：**成員版本的左邊是呼叫者；非成員版本可用明確的兩個參數處理不同運算元順序。

> **教材整理說明：**正文的主題順序與核心概念源自原先六頁講義；七關練習改編自 Codex 的《學生練習.md》，將 `Vec2` 統一為 `Point`。每關答案均自帶 `#include`、類別定義和 `main()`，不需依賴前一關的檔案；但學習時仍可逐步替同一個類別增加功能。
