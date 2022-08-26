Простой менеджер отложенных задач для Arduino в стиле функций из JavaScript

Сделал из интереса, потому что нигде такого не видел и показалось, что будет удобно его использовать в одном из проектов. Опыт написания на c++ очень небольшой, так что библиотека не претендует на что-то очень серьёзное.

## Совместимость
Не имеет зависимостей, совместим со всеми Arduino платформами (Arduino-функции не используются)

## Инициализация
`TimersDispatcher<TimeType> dispatcher(timeFunc, maxTime)`
В <TimeType> нужно передать тип данных, который возвращает функция временни
**Аргументы**
- timeFunc - функция времени, это может быть как millis или micros, так и любая кастомная функция
- maxTime - максимальное значение времени (после которого происходит сброс)
**Соответственно, библиотека работает с теми единицами измерения, которые возвращает функция времени**
```cpp
// при такой инициализации, во всех функциях нужно будет указывать значения в миллисекундах
TimersDispatcher<unsigned long> dispatcher(millis, 4294967295);

void setup() {

}

// чтобы библиотека работала, в цикле программы нужно вызвать метод loop
// его можно помещать в условия или задержки,
// но чем чаще он будет срабатывать тем, точнее будут вызываться задачи 
void loop() {
  dispatcher.loop();
}
```

## Использование
В библиотеке есть всего 3 метода
- [`TaskId* setTimeout(bool callback, delayTime)`](#setTimeout)
- [`TaskId* setInterval(bool callback, intervalTime)`](#setInterval)
- [`void clearInterval(TaskId* id)`](#clearInterval)

<a id="setTimeout"></a>
### setTimeout
Один раз вызывает переданную функцию спутся указанное время
**Аргументы**
- Функция (должна возвращать bool, не важно true или false, в любом случае сработает только один раз)
- Задержка
```cpp
TimersDispatcher<unsigned long> dispatcher(millis, 4294967295);

bool someFunc() {
  Serial.println("Вызвана спустя 3000мс");
  return false;
}

void setup() {
  dispatcher.setTimeout(someFunc, 3000);
}
 
void loop() {
  dispatcher.loop();
}
```

<a id="setInterval"></a>
### setInterval
Постоянно вызывает переданную функцию через указанное время
**Аргументы**
- Функция (если вернуть true, функция больше не будет вызываться)
- Задержка

Мигание светодиодом раз в секунду
```cpp
TimersDispatcher<unsigned long> dispatcher(millis, 4294967295);

bool led;

bool blink() {
  led = !led;
  digitalWrite(13, led);
  return false;
}

void setup() {
  pinMode(13, OUTPUT);
  dispatcher.setInterval(blink, 500);
}
 
void loop() {
  dispatcher.loop();
}
```

5 миганий светодиодом
```cpp
TimersDispatcher<unsigned long> dispatcher(millis, 4294967295);

bool led;
byte count;

bool blink() {
  led = !led;
  digitalWrite(13, led);
  if (count++ > 10) {
    return true; // прекращает постоянный вызов
  }
  return false
}

void setup() {
  pinMode(13, OUTPUT);
  dispatcher.setInterval(blink, 500);
}
 
void loop() {
  dispatcher.loop();
}
```

<a id="clearInterval"></a>
### clearInterval
Отменяет вызов задачи созданной с помощью `setTimeout` или `setInterval`
В качестве аргумента принимает указатель `TaskId* id`, который возвращают `setTimeout` и `setInterval`, после отмены задачи, указатель стоит очистить `id = nullptr`
```cpp
TimersDispatcher<unsigned long> dispatcher(millis, 4294967295);

bool led;
byte count;
TaskId* blinkId;

bool blink() {
  led = !led;
  digitalWrite(13, led);
  return false;
}

bool stopBlink() {
  dispatcher.clearInterval(blinkId);
  blinkId = nullptr;
  digitalWrite(13, LOW);
}

void setup() {
  pinMode(13, OUTPUT);
  blinkId = dispatcher.setInterval(blink, 500);
  // перестать мигать через 5 секунд
  dispatcher.setTimeout(stopBlink, 5000);
}
 
void loop() {
  dispatcher.loop();
}
```