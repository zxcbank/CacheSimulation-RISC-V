| Лабораторная работа №1 | M3104                        | АОВС |
| ---------------------- |------------------------------| ---- |
| Представление чисел    | Заимкин Святослав Валерьевич | 2024 |

> [!important]  
> Не забудьте указать в шапке свои ФИО и Номер группы

## Инструментарий
> При наличии переписываем из ТЗ, если требуется, указываем свои данные (компилятор/стандарт и т.д.). 

## Что реализовано
> Пишем свой вариант и что было выполнено (упрощенная или полная версия работы). Если что-то по ТЗ не работает, то лучше указать это здесь.

## Результат работы на тестовых данных: [ссылка на последний успешный action с актуальными данными]()

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
>
> ISA:
> - как вы делали перевод из C в asm RISC-V, что из спецификации RISC-V вы узнали необходимого для этого. не забываем результат перевода приложить в виде отдельного файла (читаем README.md)
> - как реализован транслятор asm->binary (какие классы/функции писали и зачем)
> Cache:
> - результат расчёта параметров системы (формулы для расчёта, пояснения к вычислениям и какие значения по итогу получились). Все пункты, где было написано "вычислить самостоятельно".
> - какие классы/функции созданы для представления кеша, как реализованы политики вытеснения и конфигурация в целом, как он работает
> Моделирование задачи:
> - результат работы написанной программы (то, что выводится в стандартный поток вывода / файл при разных аргументах командной строки)
> - 
> ... прочее, что считаете полезным описать ...
>
>> Представьте, что вы пришли на защиту, и вам сказали "Рассказывай, что как реализовано".

### Примечания по оформлению (можно удалить после создания отчёта)

1. Не нужно вставлять своё описание как quote (цитату). 
2. Старайтесь также разбивать ваш текст на логические разделы и отделять мысли заголовками.
3. Если хочется вставить фрагменты кода, то делаете это блоком кода. Если же ссылаетесь на код, то рекомендуется это делать через permalink на файлы в репозитории.
4. Можно пользоваться всеми возможностями Github Markdown.
5. Никаких фоток в отчёте, ровно как и скринов полного экрана. Это больно смотреть \(
6. Если вы хотите вставить изображение из сети, то не нужно его грузить в репозиторий.
7. В случае описание подсхем Logisim, если вы просто ссылаетесь на подсхему в целом, то достаточно указать назвнаие подсхемы из проекта. Если же вы описываете какой-то небольшой фрагмент схемы, то лучше его вставить в виде рисунка в отчёт.