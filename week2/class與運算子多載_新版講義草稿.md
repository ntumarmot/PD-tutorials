# C++ Class 與 Operator Overloading：從一個 `Point` 開始

> Markdown 審查稿。讀者已學過 C++ 基礎語法、`struct`、函式與指標；本篇不預設讀者熟悉參考、建構子或物件導向。範例使用 C++17。

## 先備知識與教學順序

開始前，學生應能讀懂 `struct PointData { int x; int y; };`、函式的參數與回傳值、指標 `int* p` 與 `*p`。本篇會重新建立「參考」（`&`）的直覺；學過指標，不代表已經會用參考。這堂課尚不需要繼承（讓新類別沿用既有類別的成員），也不需要用 `new`／`delete` 手動管理記憶體；第 6 節才會說明物件的複製。

全篇約定 `Point(3, 4)` 是座標 `(3, 4)`。課堂七關練習使用名稱 `Vec2`，概念和這裡的 `Point` 對應；[學生練習](學生練習.md)仍讓學生從空白檔實作。

| 順序 | 先回答的問題 | 新增能力 | 對應練習 |
| --- | --- | --- | --- |
| 1. `struct` 與 `class` | 已有 `struct`，為何還要安排公開與私有成員？ | 看懂類別介面與存取限制 | 第 1 關 |
| 2. 建構子 | 如何讓新物件一建立就有座標？ | 用初始化列表建立 `Point` | 第 1 關 |
| 3. 成員函式、參考與 `const` | 哪個物件會變？如何把物件交給唯讀函式？ | 寫會修改與只讀的方法 | 第 1、2 關 |
| 4. `==`、`!=` | 兩個 `Point` 怎樣才算相等？ | 定義不修改物件的比較 | 第 3 關 |
| 5. `+=` 與 `this` | `a += b` 要改誰？如何回傳同一個 `a`？ | 修改左邊並支援連續運算 | 第 4 關 |
| 6. `+` 與回傳值 | 如何產生新結果，同時保留原物件？ | 複製、累加並回傳新物件 | 第 5 關 |
| 7. `[]` 與回傳參考 | 為何 `p[0] = 9` 能改到物件的資料？ | 區分可寫與唯讀索引 | 第 6 關 |
| 8. 類別外運算子 | 為何 `p + 3` 與 `3 + p` 不能都照同一種成員寫法？ | 定義兩種加法順序 | 第 7 關 |

前兩節要一起看：第 1 節先說明設計理由，第 2 節才完成第一個可編譯程式。此後的短程式碼都會指出要加在 `Point` 的哪裡；文末附有整合後的完整程式，方便核對與試跑。

## 1. 已有 `struct`，為什麼還要談 `class`？

### 為什麼需要

用 `struct` 收在一起，比兩個互不相關的變數清楚：

```cpp
struct PointData {
    int x;
    int y;
};

int main() {
    PointData p{3, 4};
    p.x = 8;             // 現在 p 是 (8, 4)
}
```

這段程式沒有錯。困難在於程式變大後，每個使用 `PointData` 的地方都能直接改 `x`、`y`。假設日後要記錄每次移動，或改變座標的儲存方式，就得逐一檢查那些直接碰欄位的程式。我們可以先約定：外面只提出「移動多少」或「讀取座標」的要求，欄位由型別自己的函式處理。

### 語法與實際差異

```cpp
class Point {
private:
    int x_;
    int y_;
public:
    // 建構子和公開函式，下一節加入。
};
```

`PointData p` 中的 `p`，或稍後的 `Point p`，都是由型別建立的**物件**。寫在型別裡的函式稱為**成員函式**。`private` 以下的 `x_`、`y_` 不能由 `main` 直接存取；`public` 以下的名稱是外部可用的**介面**，也就是使用這個型別時能呼叫的功能。欄位名稱尾端的 `_` 只是本講義用來區分欄位和參數的命名方式，**不是 C++ 語法**。這個設計骨架還沒給欄位初值，下一節才會變成可執行的例子。

`struct` 也能有成員函式、建構子、`private` 和運算子多載；`class` 也能把欄位設為 `public`。兩者在本課最直接的語言差異是**未標明存取權限時**：`struct` 的成員預設為 `public`，`class` 的成員預設為 `private`。另一個本課暫時用不到的差異是未標明繼承方式時，`struct` 預設公開繼承，`class` 預設私有繼承。不能說「`struct` 只有資料，`class` 才能有函式」。常見慣例是用 `struct` 表示可直接讀寫的簡單資料組合，用 `class` 表示希望經由公開函式操作的型別；這是**慣例**，不是編譯器的規定。

## 2. 建立物件時就給座標：建構子與初始化列表

### 為什麼需要

若建立 `Point` 後還得記得另外呼叫 `setX()`、`setY()`，就可能漏掉一步。建構子讓「建立物件」與「給初始座標」同時完成。以下要求每次建立 `Point` 都提供兩個整數，因此 `Point p(3, 4)` 一完成，`p` 就有 `(3, 4)`。

### 第一個完整程式

```cpp
#include <iostream>

class Point {
private:
    int x_;
    int y_;
public:
    Point(int x, int y) : x_(x), y_(y) {}

    int getX() { return x_; }
    int getY() { return y_; }
    void print() {
        std::cout << '(' << x_ << ", " << y_ << ')';
    }
};

int main() {
    Point p(3, 4);
    p.print();
    std::cout << '\n';
}
```

這個程式印出 `(3, 4)`。依執行順序讀：

1. `int x_; int y_;` 是每個 `Point` 各自持有的欄位。`Point p(3, 4);` 建立 `p`，呼叫名稱與類別相同的 `Point(int x, int y)`。建構子沒有回傳型別。
2. `: x_(x), y_(y)` 稱為**成員初始化列表**。直覺上，把它讀成「進入大括號前，先用參數 `x` 建立欄位 `x_`，用參數 `y` 建立欄位 `y_`」。左邊是物件的欄位，括號中是傳入的值。於是 `p.x_` 是 3，`p.y_` 是 4。
3. `p.print()` 讓 `p` 執行自己的 `print()`。函式讀取 `p` 的欄位，送出文字 `(3, 4)`；`p` 不變。最後 `std::cout << '\n'` 才換行，因為 `print()` 自己不換行。

為何不用在建構子的大括號內寫 `x_ = x; y_ = y;`？對這兩個 `int` 欄位，最後的數值會相同；但初始化列表在欄位建立時直接給值，大括號內的指定則在欄位建立後才執行。有些欄位（例如 `const` 欄位和參考欄位）必須在建立時初始化。**欄位真正的初始化順序依類別中的宣告順序**，不依初始化列表的書寫順序。

現在的 `getX()`、`getY()`、`print()` 還沒有唯讀保證；第 3 節會補上。

## 3. 成員函式改的是誰？`const` 和參考如何配合？

### 先看物件如何改變

寫 `p.move(1, -2)` 時，輸入是兩個位移量；要改的是點號左邊的 `p`，不是所有 `Point`。把下面兩個函式加到 `Point` 的 `public:` 區域：

```cpp
void move(int dx, int dy) {
    x_ += dx;
    y_ += dy;
}
int lengthSquared() const {
    return x_ * x_ + y_ * y_;
}
```

`move` 的兩行各修改呼叫者的一個欄位。`Point p(3, 4); p.move(1, -2);` 之後，`p=(4, 2)`；`lengthSquared()` 只計算 `4*4 + 2*2`，回傳 20，沒有再改 `p`。使用長度的平方，可以先練習只讀函式，不必牽涉開平方根。

### 成員函式尾端的 `const`

若有 `const Point fixed(3, 4);`，我們希望能讀取座標，卻不能呼叫 `fixed.move(...)` 修改它。把原本三個唯讀函式改成：

```cpp
int getX() const { return x_; }
int getY() const { return y_; }
void print() const {
    std::cout << '(' << x_ << ", " << y_ << ')';
}
```

參數列表**後面**的 `const` 是成員函式的承諾：不能透過此函式改動呼叫者的一般欄位。於是 `fixed.getX()`、`fixed.print()`、`fixed.lengthSquared()` 能編譯；`fixed.move(1, 0)` 不能。這個位置的 `const` 只適用於成員函式。

### 從參考走到 `const Point&`

參考可以先想成同一變數的另一個名稱：

```cpp
int a = 3;
int& alias = a;
alias = 8;            // a 也變成 8
```

`alias` 沒有另外存一份整數。它在建立時要連到已有的值，透過它指定就會改原值。這和指標都能碰到原資料，但使用參考時不寫 `*alias`；`&` 在此是**型別的一部分**，不是取位址運算。

物件作參數時，`Point p` 會建立一份新物件；`Point& p` 使用呼叫端原本的物件。若只需讀取，寫 `const Point& p`：不用另做一份，也禁止透過此參數呼叫 `move()` 等修改函式。

```cpp
void show(const Point& point) {
    point.print();
}

int main() {
    Point p(3, 4);
    show(p);            // 印出 (3, 4)，p 仍是 (3, 4)
}
```

這段程式接在已定義好的 `Point` 類別後面，作為一個獨立示範；它的 `main` 取代第 2 節範例的 `main`。`show` 是**類別外的一般函式**，不是 `Point` 成員。它的 `const` 修飾參數 `point`，不能在 `show(...)` 後再寫成員函式用的 `const`。`point.print()` 能編譯，是因為 `print()` 自己在參數列表後標了 `const`。

## 4. 讓 `==` 與 `!=` 比較兩個座標

### 為什麼需要

兩個分別建立的 `Point`，即使都存 `(3, 4)`，仍是兩個物件。我們自行約定：只有 `x`、`y` **都相同**才算相等。C++ 允許替自訂型別定義既有運算子的行為，稱為**運算子多載**；它不會自行猜出「相等」的意思。

### 寫法與呼叫過程

把以下函式加入 `Point` 的 `public:` 區域：

```cpp
bool operator==(const Point& other) const {
    return x_ == other.x_ && y_ == other.y_;
}
bool operator!=(const Point& other) const {
    return !operator==(other);
}
```

`a == b` 可先看成 `a.operator==(b)`。`a` 是左邊的呼叫者，`b` 進入右邊參數 `other`。第一行的 `bool` 表示結果是真或假；`const Point& other` 表示不複製也不透過函式修改右邊；最後的 `const` 表示不修改左邊。`Point` 的成員函式能讀另一個 `Point` 的私有欄位，因此 `other.x_` 合法。

`return x_ == other.x_ && y_ == other.y_;` 同時檢查兩個座標。`operator!=` 直接呼叫本物件已有的 `operator==`，再用 `!` 反轉結果，不必重寫比較條件。

```cpp
int main() {
    const Point a(3, 4), b(3, 4), c(3, 5);
    std::cout << (a == b) << ' ' << (a == c) << ' ' << (a != c);
    // 輸出：1 0 1；a、b、c 都沒有改變
}
```

`1`、`0` 是 `std::cout` 預設印出的 `bool` 值。若只比較 `x`，就會把 `(3, 4)` 與 `(3, 5)` 錯判為相等。

## 5. `a += b` 要改 `a`：回傳原物件與 `this`

### 為什麼需要

約定 `(1, 2) += (3, 4)` 的效果是左邊變成 `(4, 6)`，右邊仍是 `(3, 4)`。若希望 `(a += b) += c` 也成立，第一次運算後就要把同一個 `a` 交給第二次。

### 先建立 `this` 的直覺

`Point* ptr = &a;` 的 `ptr` 指向 `a`，`*ptr` 是 `a` 本身。執行 `a.move(...)` 一類成員函式時，C++ 也讓函式知道「現在是哪個物件在呼叫我」。這個內建指標叫 `this`；呼叫者是 `a` 時，可以先把 `this` 想成指向 `a`，`*this` 就是 `a` 這個物件。平常讀寫欄位可直接寫 `x_`；要回傳呼叫者本身時，`return *this;` 特別有用。

### 寫法與狀態變化

把以下函式加入 `Point` 的 `public:` 區域：

```cpp
Point& operator+=(const Point& other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}
```

`a += b` 可看成 `a.operator+=(b)`。第一行的 `Point&` 表示結果是對**左邊原物件**的參考，不是新物件。參數 `other` 唯讀，右邊的 `b` 不會被這段程式修改。中間兩行各加對應座標；最後回傳已改好的 `a`。

| 執行步驟 | `a` | `b` | 原因 |
| --- | --- | --- | --- |
| 開始 | `(1, 2)` | `(3, 4)` | 兩個各自存在的物件 |
| `a += b` 後 | `(4, 6)` | `(3, 4)` | 只修改左邊 |
| 再做 `a += a` 後 | `(8, 12)` | `(3, 4)` | 左右碰巧是同一物件，各座標加倍 |

若另有 `Point c(1, 1);`，`(a += b) += c` 會先改 `a`，再沿用回傳的 `a` 改第二次。此函式不能在尾端加 `const`，因為它確實改了呼叫者。

## 6. `a + b` 要產生新物件：複製與回傳值

### 為什麼需要

我們約定 `a + b` 產生新的座標和，保留原本的 `a`、`b`。既然 `+=` 會修改左邊，就先複製一份左邊，再對副本做 `+=`。

### 寫法與逐步追蹤

把以下函式加入 `Point` 的 `public:` 區域：

```cpp
Point operator+(const Point& other) const {
    Point result(*this);
    result += other;
    return result;
}
```

假設 `a=(1, 2)`、`b=(3, 4)`，執行 `Point sum = a + b;` 時：

1. `a` 呼叫 `operator+`，`other` 參考到 `b`。尾端的 `const` 限制左邊，參數的 `const` 限制透過 `other` 修改右邊。
2. `Point result(*this);` 用現有的 `a` 建立**新的同型別物件**；此時 `result=(1, 2)`。這種用現有物件建立新物件的動作稱為複製建構。當前 `Point` 只有兩個 `int` 欄位，編譯器提供的複製行為會複製這兩個數值。
3. `result += other;` 修改新物件，讓 `result=(4, 6)`。它沒有對 `a` 呼叫 `+=`，所以 `a` 仍是 `(1, 2)`。
4. `return result;` **以值回傳**，`sum=(4, 6)`；`b` 仍是 `(3, 4)`。不能回傳 `Point&` 指向區域變數 `result`，因為函式結束後那個區域變數的生命期便結束。

`Point a(1, 2), b(3, 4), c(5, 6); Point d = a + b + c;` 先算 `(4, 6)`，再加 `c` 得 `(9, 12)`，三個原物件都不變。這裡的複製說明只針對目前兩個整數欄位的 `Point`；若類別日後自行管理動態配置的記憶體，必須重新檢查複製行為。

## 7. `p[0] = 9` 為什麼能改到 `p`？

### 為什麼需要

約定 `p[0]` 是 `x`、`p[1]` 是 `y`。讀取只需得到數值；但 `p[0] = 9` 必須寫回 `p` 內部的 `x_`。若 `operator[]` 回傳普通 `int` 值，得到的只是數值，不能藉它修改原欄位。

```cpp
int x = 3;
int copy = x;
copy = 9;             // x 仍是 3：改的是副本
int& same = x;
same = 9;             // x 變成 9：改的是原值
```

### 可寫與唯讀的兩個版本

先在檔案最上方加入 `#include <cstdlib>`，供 `std::exit` 使用。再把兩個函式加入 `Point` 的 `public:` 區域：

```cpp
int& operator[](int index) {
    if (index == 0) return x_;
    if (index == 1) return y_;
    std::cerr << "index 只能是 0 或 1\n";
    std::exit(1);
}
int operator[](int index) const {
    if (index == 0) return x_;
    if (index == 1) return y_;
    std::cerr << "index 只能是 0 或 1\n";
    std::exit(1);
}
```

讀第一個版本時，先看輸入 `index`：值為 0 就回傳欄位 `x_` 的參考，值為 1 就回傳 `y_` 的參考。若兩者都不是，`std::cerr` 印出錯誤，`std::exit(1)` 結束程式。第二個版本檢查同樣的索引，但回傳的是整數值，不提供修改欄位的途徑。

對一般物件 `Point p(3, 4);`，`p[0] = 9` 使用第一個版本：找到 `x_`、回傳對它的參考，再把 9 寫進該欄位。執行後 `p=(9, 4)`。即使只是讀 `p[1]`，一般物件仍可透過第一個版本取得數值 4。

對 `const Point fixed(3, 4);`，只能使用第二個版本。它回傳整數值，因此 `int y = fixed[1];` 得 4，`fixed` 不變；`fixed[1] = 9` 不能編譯。兩個函式能同名同參數，是因為其中一個的參數列表**後面**有 `const`；**不能只靠回傳型別不同來重載**。

這裡選 `int` 作索引，方便明確檢查 `-1`、`2` 等無效值。無效索引先印錯誤，再以非零狀態結束整個程式，不會回到下一行。測試 `p[2]` 時請單獨執行一次。`std::exit(1)` 是此例選用的錯誤處理方式，不表示所有 `operator[]` 都必須如此。

## 8. 為什麼 `p + 3` 與 `3 + p` 要分開寫？

### 為什麼需要

先約定「點加整數」是兩個座標都增加該整數，不改原點。例如 `(2, 5) + 3` 得到新的 `(5, 8)`。成員運算子由左邊物件呼叫：`p + 3` 的左邊是 `Point`，能寫成其成員；`3 + p` 的左邊是 `int`，不能要求整數去呼叫 `Point` 的成員函式。

### 先支援 `p + 3`

把以下函式加入 `Point` 的 `public:` 區域：

```cpp
Point operator+(int amount) const {
    return Point(x_ + amount, y_ + amount);
}
```

`p + 3` 可看成 `p.operator+(3)`。它讀 `p` 的兩個座標，各加 3，再呼叫建構子產生新物件；`p` 不變。此 `operator+(int)` 與上一節 `operator+(const Point&)` 能同名，因為右邊參數型別不同。

### 再支援 `3 + p`

把以下函式寫在 **`Point` 類別結尾的 `};` 之後、`main` 之前**：

```cpp
Point operator+(int amount, const Point& point) {
    return point + amount;
}
```

這是類別外的一般函式，有左右兩個參數。`3 + p` 把 3 傳給 `amount`、`p` 傳給 `point`；函式呼叫已有的 `point + amount`，回傳新 `Point`。它只使用公開的成員運算子，不需碰私有欄位。它也沒有成員函式專用的 `this`。此處的 `const` 修飾參數 `point`，類別外函式的括號後沒有成員函式用的 `const`。

```cpp
int main() {
    Point p(2, 5);
    Point a = p + 3;   // a 是 (5, 8)
    Point b = 3 + p;   // b 是 (5, 8)，p 仍是 (2, 5)
}
```

## 整合後的可編譯範例

以下把前面各節的 `Point` 合在一起。可在課後核對插入位置與執行結果；第一次練習仍可先只看對應節的短程式碼。

```cpp
#include <cstdlib>
#include <iostream>

class Point {
private:
    int x_;
    int y_;
public:
    Point(int x, int y) : x_(x), y_(y) {}
    int getX() const { return x_; }
    int getY() const { return y_; }
    void print() const { std::cout << '(' << x_ << ", " << y_ << ')'; }
    void move(int dx, int dy) { x_ += dx; y_ += dy; }
    int lengthSquared() const { return x_ * x_ + y_ * y_; }

    bool operator==(const Point& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
    bool operator!=(const Point& other) const {
        return !operator==(other);
    }
    Point& operator+=(const Point& other) {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }
    Point operator+(const Point& other) const {
        Point result(*this);
        result += other;
        return result;
    }
    int& operator[](int index) {
        if (index == 0) return x_;
        if (index == 1) return y_;
        std::cerr << "index 只能是 0 或 1\n";
        std::exit(1);
    }
    int operator[](int index) const {
        if (index == 0) return x_;
        if (index == 1) return y_;
        std::cerr << "index 只能是 0 或 1\n";
        std::exit(1);
    }
    Point operator+(int amount) const {
        return Point(x_ + amount, y_ + amount);
    }
};

Point operator+(int amount, const Point& point) {
    return point + amount;
}

int main() {
    Point p(3, 4);
    p.move(1, -2);
    p.print();
    std::cout << "；長度平方=" << p.lengthSquared() << '\n';

    const Point fixed(4, 2);
    std::cout << "相等=" << (p == fixed) << '\n';

    Point a(1, 2), b(3, 4);
    a += b;
    a.print();
    std::cout << '\n';

    Point sum = Point(1, 2) + b;
    sum[0] = 9;
    sum.print();
    std::cout << "；fixed[1]=" << fixed[1] << '\n';

    Point scalar = 3 + Point(2, 5);
    scalar.print();
    std::cout << '\n';
}
```

預期輸出：

```text
(4, 2)；長度平方=20
相等=1
(4, 6)
(9, 6)；fixed[1]=2
(5, 8)
```

編譯可用 `clang++ -std=c++17 -Wall -Wextra -pedantic point.cpp -o point`。上面的程式沒有呼叫錯誤索引；若想測錯誤分支，請另在測試程式中執行 `p[2]`，因為它會立刻結束程式。
