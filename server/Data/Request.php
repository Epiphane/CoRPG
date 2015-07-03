<?

/*
 * Request class file
 *
 * @author Thomas Steinke
 */
namespace Data;

class Request
{
	public $start  = 0;
	public $page   = 1;
	public $single = false;

	public $Filter = [];
	public $Sort   = [];

	public function __construct($data = null) 
	{
		if ($data)
			$this->init($data);
	}

	public function init($data) {
	}
}
