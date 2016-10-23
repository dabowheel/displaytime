if [ -f app.pid ]
then
  pid=`cat app.pid`
  if [ "$pid" ]
  then
    if [ -d /proc/$pid ]
    then
      echo the old process is $pid
      kill $pid
    else
      echo no process
    fi
  else
    echo no process
  fi
else
  echo no process
fi

spawn-fcgi -a 127.0.0.1 -p 9000 build/displaytime | sed -r 's/.+PID: //' > app.pid && \
echo the new process is `cat app.pid`