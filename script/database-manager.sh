#!/bin/bash

show_help() {
    echo "Usage: $0 {start|stop|help}"
    echo
    echo "  start   启动 mariadb 和 redis"
    echo "  stop    停止 mariadb 和 redis"
    echo "  help    显示帮助信息"
}

case "$1" in
    start)
        echo "启动 mariadb..."
        sudo systemctl start mariadb

        echo "启动 redis..."
        sudo systemctl start redis

        echo "全部启动完成。"
        ;;
    stop)
        echo "停止 mariadb..."
        sudo systemctl stop mariadb

        echo "停止 redis..."
        sudo systemctl stop redis

        echo "全部停止完成。"
        ;;
    h|help|"")
        show_help
        ;;
    *)
        echo "未知参数: $1"
        show_help
        exit 1
        ;;
esac
