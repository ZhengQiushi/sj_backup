#1652238 �����
#!/bin/bash
if [ $# == 2 ]; then
    cat $1|grep -v "^ *#"|awk '$2>""{print $1,$2}' | while read line
    do
        useradd $(echo "u"$line|awk '{print $1}')
        echo '�û� '$(echo "u"$line|awk '{print $1}') $(echo "u"$line|awk '{print $2}')' ����ӣ�'
        echo $(cat $2) | passwd $(echo "u"$line|awk '{print $1}') --stdin > /dev/null 2>&1
        echo "passwd" > $2
        echo "��ʹ��$2���ó�ʼ���룬�״ε�½�����޸�����"
    done 
else
    echo "����������Ϊ2������"
fi