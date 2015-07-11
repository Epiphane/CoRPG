<?

/*
 * ActionController class file
 *
 * @author Thomas Steinke
 */
namespace Action\Controller;

use \Action\Model\ActionModel;

class ActionController
{
	private $actor, $victim;

	private function check($var, $name = "variable") {
		if ($var == null) {
			throw new \Exception($name . " is not defined");
		}
	}

	public function __construct($actor, $victim) {
		$this->actor = $actor;
		$this->victim = $victim;
	}

	/**
	 * @params JSON action
	 */
	public function act($action) {
		$method = $action["method"];

		if (method_exists($this, $method)) {
			unset($action["method"]);
			try {
				$this->check($this->actor, "actor");
				return $this->$method($action);
			}
			catch (\Exception $e) {
				return [
					"success" => false,
					"error" => $e->getMessage
				];
			}
		}
		else {
			return [
				"success" => false,
				"error" => "Method '" . $method . "' not found."
			];
		}
	}

	public function set($action) {
		return $this->actor->update($action);
	}

	public function attack($action) {
		$this->check($this->victim, "victim");
		$this->check($action["damage"], "action.damage");

		$newHealth = $this->victim->health - $action["damage"];
		if ($this->victim->health > 0 && $newHealth <= 0) {
			$this->kill_victim();
		}

		if ($newHealth < 0) $newHealth = 0;

		$result = $this->victim->update([
			"health" => $newHealth
		]);

		return [
			"success" => $result
		];
	}

	private function kill_victim() {
		$experience = $this->victim->level;

		$currentExperience = $this->actor->experience;
		$currentLevel = $this->actor->level;		


		$newExperience = $currentExperience + $experience;
		$experienceNeeded = $currentLevel * 10;
		while ($newExperience >= $experienceNeeded) {
			$newExperience -= $experienceNeeded;

			$currentLevel ++;			
			$experienceNeeded = $currentLevel * 10;
		}

		$res = $this->actor->update([
			"level" => $currentLevel,
			"experience" => $newExperience
		]);

		if (!$res)
			throw new \Exception("Actor update failed");
	}
}
