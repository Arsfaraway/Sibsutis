#!/bin/bash

# Дата
date=$(date)

# Имя учетной записи
username=$(whoami)

# Доменное имя ПК
hostname=$(hostname)

# Процессор
processor_model=$(lscpu | grep "Model name" | awk -F: '{print $2}' | xargs)
processor_architecture=$(lscpu | grep "Architecture" | awk -F: '{print $2}' | xargs)
max_cpu_frequency=$(lscpu | grep "CPU max MHz" | awk -F: '{print $2}' | xargs)
current_cpu_frequency=$(cat /proc/cpuinfo | grep "cpu MHz" | awk -F: '{print $2}' | uniq)     
num_cores=$(lscpu | grep "Core(s) per socket" | awk -F: '{print $2}' | xargs)
num_threads=$(lscpu | grep "Thread(s) per core" | awk -F: '{print $2}' | xargs)
cpu_load=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}' | xargs)

# Оперативная память
mem_info=$(free -h)
mem_cache_l1=$(cat /sys/devices/system/cpu/cpu0/cache/index0/size)
mem_cache_l2=$(cat /sys/devices/system/cpu/cpu0/cache/index2/size)
mem_cache_l3=$(cat /sys/devices/system/cpu/cpu0/cache/index3/size)
mem_total=$(echo "$mem_info" | grep "Mem:" | awk '{print $2}')
mem_available=$(echo "$mem_info" | grep "Mem:" | awk '{print $7}')

# Жесткий диск
disk_info=$(df -h /)
disk_total=$(echo "$disk_info" | awk 'NR==2 {print $2}')
disk_available=$(echo "$disk_info" | awk 'NR==2 {print $4}')
num_partitions=$(df -h | grep -c '^/dev/')

# SWAP
swap_info=$(free -h | grep "Swap:")
swap_total=$(echo "$swap_info" | awk '{print $2}')
swap_available=$(echo "$swap_info" | awk '{print $3}')

# Сетевые интерфейсы
num_interfaces=$(ip -o link show | wc -l)
interfaces_info=$(ip -o link show)

echo "Дата: $date"
echo "Имя учетной записи: $username"
echo "Доменное имя ПК: $hostname"
echo "Процессор:"
echo "Модель: $processor_model"
echo "Архитектура: $processor_architecture"
echo "Тактовая частота максимальная: $max_cpu_frequency MHz"
echo "Тактовая частота текущая: $current_cpu_frequency MHz"
echo "Количество ядер: $num_cores"
echo "Количество потоков на одно ядро: $num_threads"
echo "Загрузка процессора: $cpu_load%"
echo "Оперативная память:"
echo "Cache L1: $mem_cache_l1"
echo "Cache L2: $mem_cache_l2"
echo "Cache L3: $mem_cache_l3"
echo "Всего: $mem_total"
echo "Доступно: $mem_available"
echo "Жесткий диск:"
echo "Всего: $disk_total"
echo "Доступно: $disk_available"
echo "Количество разделов: $num_partitions"
echo "Смонтировано в корневую директорию /:"
echo "Объём неразмеченного пространства: $(fdisk -l | grep "Disk /dev/sda" | awk '{print $3}')"
echo "SWAP всего: $swap_total"
echo "SWAP доступно: $swap_available"
echo "Сетевые интерфейсы:"
echo "Количество сетевых интерфейсов: $num_interfaces"
echo "$interfaces_info" | while read -r line; do
    interface_name=$(echo "$line" | awk -F': ' '{print $2}')
    interface_mac=$(ip link show "$interface_name" | awk '/link/ {print $2}')
    interface_ip=$(ip addr show "$interface_name" | awk '/inet / {print $2}')
    interface_speed=$(ethtool "$interface_name" | grep "Speed:" | awk '{print $2}')
    interface_max_speed=$(ethtool "$interface_name" | grep "Supported ports:" | awk '{print $3}')
    echo "Имя: $interface_name"
    echo "MAC: $interface_mac"
    echo "IP: $interface_ip"
    echo "Стандарт связи: $(ethtool "$interface_name" | grep "Supported ports:" | awk '{print 
$2}')"
    echo "Максимальная скорость соединения: $interface_max_speed"
    echo "Фактическая скорость соединения: $interface_speed"
done