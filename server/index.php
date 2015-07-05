<?php

require_once("config.php");
require_once("Data/DAO.php");
require_once(__DIR__ . "/GameObject/Model/GameObjectPropertyModel.php");
require_once(__DIR__ . "/GameObject/Model/GameObjectModel.php");

$url = substr($_SERVER["REQUEST_URI"], strlen("/CoRPG/"));
if (strrpos($url, "?"))
	$url = substr($url, 0, strrpos($url, "?"));
$path = explode("/", $url);

$deps = explode(",", $_GET["deps"]);
if ($deps[count($deps) - 1] == "") {
	array_splice($deps, -1);
}

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
		array_push($deps, $path[0]);
		$request->Filter[] = new \Data\Filter("name", $path[1]);
	}

	if (count($deps) > 0) {
		$request->Filter[] = new \Data\InFilter("region", $deps);
	}

	$obj = \GameObject\Model\GameObjectModel::findOne($request);

	if ($obj)
		echo json_encode($obj->read());
	else
		echo "{}";
}

?>
