<?php

$host = "192.168.1.72:3306";
$dbname = "esp32";
$username = "esp32mpu";
$password = "";

try 
{
    // CRIAÇÃO DE UM PDO COM AS CREDENCIAIS DA DB
    $dbh = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    // PDO ERRO HANDLE
    $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // IR BUSCAR DADOS DE HTTP + GET REQUEST ESP32
    $acceleration_x = isset($_GET['acceleration_x']) ? $_GET['acceleration_x'] : 0.2;
    $acceleration_y = isset($_GET['acceleration_y']) ? $_GET['acceleration_y'] : 0.2;
    $acceleration_z = isset($_GET['acceleration_z']) ? $_GET['acceleration_z'] : 0.2;
    $gyro_x = isset($_GET['gyro_x']) ? $_GET['gyro_x'] : 0.069;
    $gyro_y = isset($_GET['gyro_y']) ? $_GET['gyro_y'] : 0.069;
    $gyro_z = isset($_GET['gyro_z']) ? $_GET['gyro_z'] : 0.069;
    $estado = isset($_GET['estado']) ? $_GET['estado'] : 0.069;

    // INSERIR DADOS
    $stmt = $dbh->prepare("INSERT INTO mpu6050 (acceleration_x, acceleration_y, acceleration_z, gyro_x, gyro_y, gyro_z, estado) VALUES (:acceleration_x, :acceleration_y, :acceleration_z, :gyro_x, :gyro_y, :gyro_z, :estado)");
    $stmt->bindParam(':acceleration_x', $acceleration_x);
    $stmt->bindParam(':acceleration_y', $acceleration_y);
    $stmt->bindParam(':acceleration_z', $acceleration_z);
    $stmt->bindParam(':gyro_x', $gyro_x);
    $stmt->bindParam(':gyro_y', $gyro_y);
    $stmt->bindParam(':gyro_z', $gyro_z);
    $stmt->bindParam(':estado', $estado);
    $stmt->execute();

    // MENSAGEM SUCESSO
    echo "Dados inseridos com sucesso";
} 
catch(PDOException $e) 
{
    // MENSAGEM INSUCESSO
    echo "Conexão falhou: " . $e->getMessage();
}
?>
