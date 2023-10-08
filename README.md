# Описание

Это репозиторий с выполнением лаб по курсу «Операционные системы» в
университете ИТМО ФПИиКТ СППО СПО.

Этот курс является экспериментальным в данный год.

Лабораторные работы основываются на модификации исходного кода ОС xv6, поэтому
репозиторий был форкнут и версии каждой ЛР будут помещаться в отдельные ветки:

- [lab-1](https://github.com/e1turin/itmo-os-xv6-riscv/tree/lab-1) (ping-pong: pipe, fork)
- ...


# Комментарии от разработчиков

- (см. [README](/README))

# Комментарии от меня

## Начало

Для того чтобы попробовать xv6 без особой возни можно использовать Docker-образ:

- https://hub.docker.com/r/wtakuo/xv6-env

Для удобного выполнения лаб было принято решение сделать «форк» (тупо копия) из
офф. репозитория от последнего коммита (mit-pdos/xv6-riscv@f5b93ef), чтобы быть
свободнее в управлении репозиторием. И каждая лаба теперь будет разрабатываться
в отдельной ветке, ссылка на которую будет добавляться в ветку `main`.

Дополнительные материалы от преподавателя по поводу работы на ARM процессорах:

- https://github.com/Pivnoy/os-dev-env

Примеры выполнения работ у других студентов

- https://github.com/Hyperb0rean/itmo_os (+rust?)
- https://github.com/orgs/vityaman-edu/... (![](http.cat/501))
- https://github.com/NorthCapDiamond/Operating-Systems-xv6

