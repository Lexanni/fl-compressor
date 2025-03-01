﻿*************************************************
*      Fast Lossless Compressor version 0.1
*************************************************

---------- Руководство пользователя -------------

Данное руководство посвящено демонстрационной программе fl-compressor, выполняющей сжатие и восстановление набора PGM-файлов, которые трактуются как спектральные слои гиперспектрального изображения. Программа не имеет графического интерфейса. Все взаимодействие с программой осуществляется через терминал посредством запуска исполняемого файла с передачей ему аргументов командной строки.
Программа имеет два режима работы: сжатие и восстановление, которые определяются, соответственно, ключами --compress и 
--decompress.
Для режима сжатия формат команды выглядит следующим образом:

	> .\fl-compressor.exe --compress <список файлов> --output <имя выходного файла> [опции]

Список файлов можно задавать как простым перечислением, так и с использованием масок. Допустимы изображения в формате PGM, имеющие одинаковые размеры и количество бит на пиксель. Имя выходного файла может быть любым, но рекомендуется использовать для выходного файла расширение .fl. Вместо просто имени файла можно задать абсолютный или относительный путь до него, но важно убедиться в существовании данного пути, так как программа не может создавать каталоги. В качестве примера приведем следующую команду:

	> .\fl-compressor.exe --compress 0000.pgm 0001.pgm 0002.pgm --output compressed.fl

После выполнения данной команды, программа попытается найти в текущем каталоге файлы 0000.pgm, 0001.pgm и 0002.pgm, выполнить их сжатие с параметрами по-умолчанию и сохранить результат в файл compressed.fl. При невозможности открыть какой-либо из указанных файлов, программа выведет соответствующее сообщение об ошибке, но не завершится, а выполнит сжатие успешно загруженных файлов.

Для режима сжатия, через аргументы командной строки можно сообщить программе требуемые параметры работы модуля предсказания и энтропийного кодера путем указания необходимых числовых значений после соответствующих аргументов-ключей. Допустимы следующие ключи:
– ключ -U определяет предел длины унарной части кодового слова  (допустимы значения от 1 до 20);
– ключ -Р задает количество спектральных слоев, используемых адаптивным предсказателем, (допустимы значения от 0 до 15);
– ключ -t_inc определяет скорость адаптации вектора весов ко входному набору данных (допустимы значения от 0 до 15);
– ключ -nu_min устанавливает нижнюю границу значения масштабирующей экспоненты  (допустимы значения от 0 до 15);
– ключ -nu_max устанавливает верхнюю границу значения масштабирующей экспоненты (допустимы значения от 0 до 15);
– ключ -Om определяет ширину диапазона значений весового вектора   (допустимы значения от 0 до 15);
– ключ -G определяет начальное значение величины счетчика элементно-адаптивного энтропийного кодера  (допустимы значения от 1 до 16);
– ключ -K устанавливает начальное значение константы инициализации аккумулятора элементно-адаптивного энтропийного кодера  (допустимы значения от 0 до 15).
Например, если требуется выполнить сжатие всех PGM-файлов в папке pgm_files в файл compressed.fl с параметрами  и, команда будет выглядеть следующим образом:

	> .\fl-compressor.exe --compress .\pgm_files\*.pgm --output compressed.fl –U 10 -P 5

Для режима восстановления формат команды выглядит следующим образом:

	> .\fl-compressor.exe --decompress <путь до сжатого файла> --output <путь к папке назначения>

Программа за один запуск может распаковать лишь один сжатый файл.   В папке назначения будут созданы файлы с именем в формате <номер спектрального слоя>.pgm. Например, команда для распаковки файла compressed.fl в каталог restored будет выглядеть следующим образом:

	> .\fl-compressor.exe --decompress compressed.fl --output .\restored\

Для работы программы в режиме восстановления не существует каких-либо дополнительных ключей-аргументов.
