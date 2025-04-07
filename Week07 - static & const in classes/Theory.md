# static 

Когато маркираме някой член като `static`, декларираме че той принадлежи на самия клас, а не на конкретен обект. Това важи както за променливи, така и за функции.

**Rectangle.h**
```cpp
class Rectangle
{
public:
    Rectangle(double x, double y, double width, double height);

    static int getInstanceCount();

private:
    double x, y, width, height;

    static int instanceCount;
};
```

**Rectangle.cpp**
```cpp
int Rectangle::instanceCount = 0;

Rectangle::Rectangle(double x, double y, double width, double height)
    : x(x), y(y), width(width), height(height)
{
    instanceCount++;
}

int Rectangle::getInstanceCount()
{
    return instanceCount;
}
```

**main.cpp**
```cpp
//...
std::cout << Rectangle::getInstanceCount() << '\n';

Rectangle rect1(0,0,240,400);

std::cout << Rectangle::getInstanceCount() << '\n';

Rectangle rect2(50,100,10,200);

std::cout << Rectangle::getInstanceCount();
```

*Output:*
```
0
1
2
```

## Особености
Когато използваме `static`, трябва задължително да зададем някаква стойност **извън дефиницията на класа***, като е най-коректно това да се случва в отделен `.cpp` файл. 

```cpp
class Rectangle
{
    //...
    static int instanceCounter = 0; // Error
}
```

```cpp
class Rectangle
{
    //...
    static int instanceCounter;
}

int Rectangle::instanceCounter = 0; // No error, but not OK if in a header file
```

---

`static` функции могат да достъпват **само** `static` член данни и други `static` функции.

От друга страна нормалните методи могат да достъпват `static` член данни и функции.

# const

Когато маркираме член данна като `const`, стойност можем да ѝ дадем само като подразбираща се такава или в инициализиращия списък на конструктор и никога след това. 

```cpp
class Rectangle
{
public:
    Rectangle(int id, double x, double y, double width, double height)
        : id(id), x(x), y(y), width(width), height(height)
    {
    }

    int getId() const
    {
        return id;
    }

private:
    double x, y, width, height;

    const int id;
};
```

## Особености

Както се очаква, не можем да направим setter за `const` поле.

---

Когато зададем едно поле като `const`, автоматичния `operator=` не се генерира (маркира се като `delete`). Съответно ако искаме да имаме такъв, трябва да си го напишем, като не пропуснем константното поле.

```cpp
class Rectangle
{
public:
    Rectangle(int id, double x, double y, double width, double height)
        : id(id), x(x), y(y), width(width), height(height)
    {
    }

    Rectangle& operator=(const Rectangle& other)
    {
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;

        return *this;
    }

    int getId() const
    {
        return id;
    }

private:
    double x, y, width, height;

    const int id;
};
```

---

Ако *целочислено* поле е маркирано едновременно като `static` и като `const`, то тогава може да бъде дефинирано още вътре в класа.

```cpp
class Rectangle
{
    // ...

    static const int NUM_SIDES = 4; // OK
};