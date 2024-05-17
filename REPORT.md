| Лабораторная работа №1 | M3104                        | АОВС |
| ---------------------- |------------------------------| ---- |
| Представление чисел    | Заимкин Святослав Валерьевич | 2024 |

## Инструментарий
> При наличии переписываем из ТЗ, если требуется, указываем свои данные (компилятор/стандарт и т.д.). 

## Что реализовано
> Вариант 1. Реализована normal версия работы: перевел программу из с в ассемблер riskv32. Также Посчитал lru-cache и plru-cache.
> Также дописал некоторые функции, о которых будет сказано дальше, чтобы перевести ассемблер в машинный код.

## Результат работы на тестовых данных: [ссылка на последний успешный action с актуальными данными](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/actions/runs/9116214943)

# Описание:
    Параметры системы:
| Параметры                | Системы                     |
|--------------------------|-----------------------------|
| MEM_SIZE                 | 262144 байт                 |
| ADDR_LEN                 | **18 бит**                  | 
| Представление чисел      | **look-through write-back** | 
| Политики вытеснения кэша | **LRU и bit-pLRU**          | 
| CACHE_WAY                | **4**                       |
| CACHE_TAG_LEN            | 8 бит                       |
| CACHE_INDEX_LEN          | 5 бит                       |
| CACHE_OFFSET_LEN         | 5 бит                       | 
| CACHE_SIZE               | 4096 байт                   |
| CACHE_LINE_SIZE          | **32 байт**                 |
| CACHE_LINE_COUNT         | 128                         |
| CACHE_SETS               | **32**                      |


> Раздел, в котором вы описываете свою реализацию:
> Подсчет параметров системы: 
> MEMSIZE = 2 ** ADDR_LEN,
CACHE_INDEX_LEN = log(CACHE_SETS) = 5 бит,
CACHE_LINE_COUNT = CACHE_SETS * CACHE_WAY = 128,
CACHE_OFFSET_LEN = log(CACHE_SIZE) = 5 бит,
CACHE_SIZE = CACHE_LINE_SIZE * CACHE_LINE_COUNT = 4096 байт.

# >` ISA:`

> Я понял, что надо разобраться с тем, как делать циклы. Понял, что в нашей ситуации логично использовать blt r1, r2, offset, 
> так как это команда описывает нужный условный переход для нашего цикла for. Далее я подумал над тем, как нам хранить константы,
> они нам нужны для умножения и blt. Я понял что их можно с помощью addi r1, r2, imm занести в любой разрешенный регистр. Вторым операндом 
> достаточно указать ZERO, тк в этом регистре всегда 0, а третьим операндом нужное число. Но не все числа из-за их длинны можно было так 
> перенести в регистры, поэтому приходилось пользоваться lui, но он это делает со сдвигом влево на 12. Поэтому потом приходилось сдвигать
> обратно на 12 с помощью srli. Далее использовав арифтемику указателей, я увеличивал соответствующие переменные на их размер в байтах.
> Перед blt я использовал инкрементирование переменной, которая используется в цикле.

# > `asm->binary :`

После [парсинга команд ассемблера](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/processor.cpp#L35) у меня получался вектор объектов AssmblerInstruction. Этот объект представляет из себя строки для команды
и трех операндов. Если команда на 2 операнда, то в третьей лежит мусор, который я не буду использовать. Поэтому оставалось просто операнды
перевести в регистры, числа или числа из памяти. Это реализуют [getRegNum()](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/regesters.cpp#L8) с помощью соответствующего enum'а или функции [convert](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/RiskVcommand.hpp#L49), которая
переводит числа из 2сс и 16сс в 10сс. После того, как операнды переведены в числовой вид, оставалось просто взять опкоды и типы команд, и аккуратно
записать в соответствующий файл. Этот функционал представлен [здесь](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/processor.cpp#L146)
Также надо было поменять порядок байт (берем по два, внутри их не разворачиваем, и меняемих местами 1 => 4, 2 => 3).
Этот функционал представлен [здесь](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/processor.cpp#L141).
Использование самой функции представлено [здесь](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/processor.cpp#L108).
Также же хотел добавить, что реализованы метки. Хотя в итоговом asm файле я их не использую по причине того, что сложно делать дебаг. 
Изначально я делал с ними, так как мне казалось, что так проще, но потом я отказался от них, но все-таки решил доделать начатое.

# `Cache:`

Для посчета кеша ([pLRU](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/Plru.hpp#L47) и [LRU](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/Lru.hpp#L36)) созданы два класса.
Они содержат в себе вектор объектов [Lru кешлинии](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/Lru.hpp#L12) и [bitPlru кеш линии](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/Plru.hpp#L11)
Эти классы могут проверять, есть ли данные в кеше или нет. При необохдимости проводить вытеснение. Они **не считают** попадания.
Попадания считаются в процессе [исполнения команд ассемблера. ](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/processor.cpp#L54)
Там выполняется проверка: [использует ли данная команда память](https://github.com/skkv-itmo-comp-arch/se-comp-arch24-cache-isa-zxcbank/blob/6b89906b572af1b15b81b0f29d8e26c74edb80a2/include/RiskVcommand.cpp#L154), если да, то мы начинаем подсчет попаданий. В зависимости от запроса (на 1, 2 или 4 байта).
Если хотя бы одного из n-байтов нет в кеше, то это мисс, иначе - хит. Так для обоих классов.

# > `Результат работы программы при разных аргументах командной строки`
##### --replacement 0 --asm rv32.asm --bin mama.bin
`LRU     hit rate: 99.1250% 
pLRU    hit rate: 99.4046%`

##### --replacement 1 --asm rv32.asm --bin mama.bin
`LRU     hit rate: 99.1250%`

##### --replacement 2 --asm rv32.asm --bin mama.bin
`pLRU    hit rate: 99.4046%`
