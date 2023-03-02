<?php

$host = "192.168.1.72:3306";
$dbname = "esp32";
$username = "esp32max";
$password = "";

try 
{
    // CRIAÇÃO DE UM PDO COM AS CREDENCIAIS DA DB
    $dbh = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    // PDO ERRO HANDLE
    $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // IR BUSCAR DADOS DE HTTP + GET REQUEST ESP32
    $bpm = isset($_GET['bpm']) ? $_GET['bpm'] : 0.069;
    $spo2 = isset($_GET['spo2']) ? $_GET['spo2'] : 0.069;
    $mediabpm = isset($_GET['mediabpm']) ? $_GET['mediabpm'] : 0.069;

    // INSERIR DADOS
    $stmt = $dbh->prepare("INSERT INTO max30100 (bpm, spo2, mediabpm) VALUES (:bpm, :spo2, :mediabpm)");
    $stmt->bindParam(':bpm', $bpm);
    $stmt->bindParam(':spo2', $spo2);
    $stmt->bindParam(':mediabpm', $mediabpm);
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
