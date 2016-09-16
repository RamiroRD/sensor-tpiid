> /etc/sensor-tpiid/sensor-tpiid.conf
echo "10" >> /etc/sensor-tpiid/sensor-tpiid.conf

sudo systemctl reload sensor-tpiid > /dev/null 2>&1

echo "Content-type: text/html"

echo ""

echo '<html>'

echo '<head>'

echo '<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">'

echo '<title>Nueva Frecuencia: 10</title>'

echo '</head>'

echo '<body>'

echo 'La frecuencia fue cambiada correctamente'

echo '</body>'

echo '</html>'

