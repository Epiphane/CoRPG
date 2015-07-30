<?php

require_once("config.php");
require_once("Data/DAO.php");
require_once(__DIR__ . "/Action/Controller/ActionController.php");
require_once(__DIR__ . "/GameObject/Model/GameObjectPropertyModel.php");
require_once(__DIR__ . "/GameObject/Model/GameObjectModel.php");
require_once(__DIR__ . "/GameObject/Controller/GameObjectController.php");

function sendResponse($response) {
	header('Content-Type: application/json');

	echo json_encode($response);
}

$url = substr($_SERVER["REQUEST_URI"], strlen("/CoRPG/"));
if (strrpos($url, "?"))
	$url = substr($url, 0, strrpos($url, "?"));
$path = explode("/", $url);

$deps = explode(",", $_GET["deps"]);
if ($deps[count($deps) - 1] == "") {
	array_splice($deps, -1);
}

$params = json_decode(file_get_contents("php://input"), TRUE);

if ($_SERVER["REQUEST_METHOD"] == "GET") {
	if ($path[0] === "") {
		echo '{"world":"Weebly"}';
	}
	else {
		$name = $region = $game = null;

		if ($path[0] === "possessions") {
			array_splice($path, 0, 1);
			$fetch_possessions = true;
		}

		// Find Object
		if (count($path) == 1) {
			$name = $path[0];
			// Just the name
		}
		elseif (count($path) == 2) {
			// Region and name!
			array_push($deps, $path[0]);
			$name = $path[1];
		}

		$obj = \GameObject\Model\GameObjectModel::findById($name);

		if (!$obj) {
			$obj = \GameObject\Model\GameObjectModel::findByNameRegionGame($name, $deps, $game);
		}
	
		if ($fetch_possessions) {
			if ($obj) {
				$possessions = array();
				foreach ($obj->ownerships as $object) {
					$possession = \GameObject\Model\GameObjectModel::findById($object->subject_id);

					$possessions[] = $possession->read();
				}

				sendResponse($possessions);
			}
			else {
				header('Content-Type: application/json');
				echo "[]";
			}
		}
		elseif ($obj) {
			sendResponse($obj->read());
		}
		else {
			header('Content-Type: application/json');
			echo "{}";
		}
	}
}
else if ($_SERVER["REQUEST_METHOD"] == "POST") {
	if ($path[0] === "action") {
		$name = $params["self"]["name"];
		$region = $params["self"]["region"];
		$actor = \GameObject\Model\GameObjectModel::findByNameRegionGame($name, array($region));

		if ($params["other"]) {
			$name = $params["other"]["name"];
			$region = $params["other"]["region"];
			$victim = \GameObject\Model\GameObjectModel::findByNameRegionGame($name, array($region));
		}

		$action = $params["action"];

		$result = \GameObject\Controller\GameObjectController::act($actor, $victim, $action);

		if ($result["success"]) {
			sendResponse([
				"result" => $result,
				"self" => $actor->read(),
				"other" => $victim ? $victim->read() : "{}"
			]);
		}
		else {
			sendResponse($result);
		}
	}
}
else if ($_SERVER["REQUEST_METHOD"] == "PUT") {
	$name = $params["name"];
	$region = $params["region"];
	$obj = \GameObject\Model\GameObjectModel::findByNameRegionGame($name, array($region));
	if ($obj) {
		sendResponse($obj->update($params));
	}
	else {
		$obj = \GameObject\Model\GameObjectModel::build($params);
	
		sendResponse($obj->save());
	}
}

?>
