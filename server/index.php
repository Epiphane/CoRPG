<?php

require_once("config.php");
require_once("Data/DAO.php");
require_once(__DIR__ . "/GameObject/Model/GameObjectPropertyModel.php");
require_once(__DIR__ . "/GameObject/Model/GameObjectModel.php");

$path = explode("/", substr($_SERVER["REQUEST_URI"], strlen("/CoRPG/")));
$deps = $_GET["deps"];

if ($path[0] === "") {
	echo '{"world":"Weebly"}';
}
else {
	$request = new \Data\Request();

	// Find Object
	if (count($path) == 1) {
		// Just the name
		$request->Filter[] = new \Data\Filter("name", $path[0]);
	}
	elseif (count($path) == 2) {
		// Region and name!
		$request->Filter[] = new \Data\Filter("region", $path[0]);
		$request->Filter[] = new \Data\Filter("name", $path[1]);
	}

	$obj = \GameObject\Model\GameObjectModel::findOne($request);

	echo json_encode($obj->read());
}

?>
