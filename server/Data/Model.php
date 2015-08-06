<?php
/*
 * Model class file
 *
 * @author Thomas Steinke
 */
namespace Data;

use \Data\DAO;

class Model
{
	public static $tableName;
	public static $pKey = null;
	public static $columns = [];
	public static $const_columns = [];

	public $_new = true;

	public static function build($assoc) {
		$m = get_called_class();
		$model = new $m();

		foreach($assoc as $col => $val) {
			$model->$col = $val;
		}

		// Create primary key if not exists
		$pKey = self::getPrimaryKey($m);
		if (!$model->$pKey) {
			$model->$pKey = $model->createPrimaryKey();

			// COLLISION CHECK
			$runs = 1;
			while($m::findById($model->$pKey)) {
				_log("Collision on Primary key " . $pKey . "=" . $model->$pKey);

				$model->$pKey = $model->createPrimaryKey($runs++);

				if ($runs === 10) {
					_log("10 collisions, aborting..");

					die();
				}
			}
		}

		return $model;
	}

	public static function getPrimaryKey($m) {
		$key = $m::$pKey ?: $m::$const_columns[0];
		if (!$key) {
			reset($m::$columns);
			$key = key($m::$columns);
		}
		return $key;
	}

	public function createPrimaryKey($rerun = null) {
		return mt_rand(100000000, 999999999);
	}

	public function save() {
		$model = get_called_class();
		$dao = new \Data\DAO($model);
	
		if ($this->_new) {
			return $dao->create($this);
		}
		else {
			return $dao->update($this, $model);
		}
	}

	public function read() {
		return $this;
	}

	public function update($attrs) {
		foreach(self::$const_columns as $key) {
			unset($attrs[$key]);
		}

		$dao = new \Data\DAO(get_called_class());

		return $dao->update($this, $attrs);
	}

	public static function findById($id) {
		$request = new \Data\Request();
		$pKey    = self::getPrimaryKey(get_called_class());
		$request->Filter[] = new \Data\Filter($pKey, $id);

		return self::findOne($request);
	}

	public static function find($request) {
		$dao = new \Data\DAO(get_called_class());

		return $dao->find($request);
	}

	public static function findOne($request) {
		$dao = new \Data\DAO(get_called_class());

		return $dao->findOne($request);
	}
}
