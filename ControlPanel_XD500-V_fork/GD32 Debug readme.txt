1. В Eclipse в настройках "Debug configurations..." изменить "Automated Generation" на "User Defined" (чтобы не перезаписывал каждый раз "ControlPanel Debug.cfg")
2. В "ControlPanel Debug.cfg" (в папке проекта) вставить строку "set CPUTAPID 0x4ba00477"
3. Всё, отладчик и загрузка из Eclipse работает