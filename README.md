# Операционные системы. ИДЗ №4
Нарин Алексей Сергеевич. БПИ 217

## Вариант 25. Условие

![image](https://github.com/alexnurin/os_idz3/assets/44980361/f9e23985-c044-402a-bc66-f6da81253ea2)

## Запуск
Необходимо сначала запустить сервер (студентка), а потом по очереди всех клиентов в разных терминалах (поклонники).


### Команды для запуска сервера:
```
gcc studentness.c -o studentness
./studentness <порт> <количество поклонников>
```

### Команды для запуска клиента:
```
gcc fan.c -o fan
./fan <ip-адрес сервера> <порт> <номер поклонника>
```

При локальном использовании следует использовать ip-адрес 127.0.0.1, а порт — целое число от 1024 до 65535, также он не должен быть занят другими процессами.

### Пример использование:
```
gcc studentness.c -o studentness
gcc fan.c -o fan
./studentness 8787 3
./fan 127.0.0.1 8787 0
./fan 127.0.0.1 8787 1
./fan 127.0.0.1 8787 2
```

## Сценарий решения
Студентка моделируется в виде сервера, поклонники в виде клиентов.

Сначала студентка ожидает, пока все поклонники "отметятся", то есть установят соединение. Затем поклонники отправляют студентке запросы, в качестве контента передавая привлекательность их предложения (генерируется случайно, в процентах)

Студентка собирает все приглашения, затем выбирает наиболее выгодное. После чего отправляет каждому поклоннику ответ — одному согласие, остальным отказ. После чего завершает соединение с каждым из поклонников. Работа сервера и клиентов на этом завершается.

Сервер и клиенты на протяжении взаимодействия выводят информацию о себе в собственную консоль.
