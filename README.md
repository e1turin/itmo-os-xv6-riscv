# Описание

Это репозиторий с выполнением лаб по курсу «Операционные системы» в
университете ИТМО ФПИиКТ СППО СПО.

Этот курс является экспериментальным в данный год.

Лабораторные работы основываются на модификации исходного кода ОС xv6, поэтому
репозиторий был форкнут и версии каждой ЛР будут помещаться в отдельные ветки:

- (см. [branches/main](https://github.com/e1turin/itmo-os-xv6-riscv))



# Комментарии от разработчиков

- (см. [README](/README))


# Задание

- (см. [TASK.md](/TASK.md))

- - -
# Комментарии

## Clangd
Чтобы нормально разрабатывать, т. е. с подсказками и линтерами, можно
подключить `clang`. Он использует 'compilation database' в виде
`compile_commands.json` (см. [rizsotto/Bear](https://github.com/rizsotto/Bear)). 
Но проблема в том, что версия 16.0.2 почему-то не умеет разрешать инклуды
заголовочных файлов по ней (в [Makefile](./Makefile) для компиляции
используются текущая директория `./`). Поэтому достаточно добавить файлик
`compile_flags.txt` в котором уже будет указан флаг `-I.`.

- Так же стоит добавить `.clangd` файл, чтобы показать корень проекта.
- Нельзя одновременно использовать `compile_commands.json` и
  `compile_flags.txt`.
- Когда `clangd` починят возможно нужно будет запускать `bear` 2 раза для
  `make` и `make qemu` (с флагом `--append`)

