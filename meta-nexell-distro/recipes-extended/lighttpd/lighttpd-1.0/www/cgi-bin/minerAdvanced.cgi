#!/bin/sh -e
#set -x

create_default_conf_file()
{
(
cat <<'EOF'
{
"pools" : [
{
"url" : "stratum+tcp://sg.stratum.slushpool.com:3333",
"user" : "maekdal.bcworker1",
"pass" : "x"
},
{
"url" : "stratum+tcp://sg.stratum.slushpool.com:3333",
"user" : "maekdal.bcworker2",
"pass" : "x"
},
{
"url" : "stratum+tcp://sg.stratum.slushpool.com:3333",
"user" : "maekdal.bcworker3",
"pass" : "x"
}
]
,
"api-listen" : true,
"api-network" : true,
"api-allow" : "W:0/0",
"temp-cutoff" : 100,
"tsb1101-volt": "400"
}

EOF
) > /config/cgminer.conf
}

if [ ! -f /config/cgminer.conf ] ; then
    if [ -f /config/cgminer.conf.factory ] ; then
		cp /config/cgminer.conf.factory /config/cgminer.conf
    else
		create_default_conf_file
    fi
fi

nexell_result=`cat /config/cgminer.conf`

# CGI output must start with at least empty line (or headers)
printf "Content-type: text/html\r\n\r\n"

cat <<-EOH
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Content-Script-Type" content="text/javascript" />
<meta http-equiv="cache-control" content="no-cache" />
<link rel="stylesheet" type="text/css" media="screen" href="/css/cascade.css" />
<!--[if IE 6]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie6.css" /><![endif]-->
<!--[if IE 7]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie7.css" /><![endif]-->
<!--[if IE 8]><link rel="stylesheet" type="text/css" media="screen" href="/css/ie8.css" /><![endif]-->
<script type="text/javascript" src="/js/xhr.js"></script>
<script type="text/javascript" src="/js/jquery-1.10.2.js"></script>
<script type="text/javascript" src="/js/json2.min.js"></script>
<script>
EOH

echo "nexell_data = ${nexell_result};"

cat <<EOT
function f_get_miner_conf() {
	try
	{
		document.getElementById("nexell_cutoff").value=nexell_data["temp-cutoff"];
		document.getElementById("nexell_voltage").value=nexell_data["tsb1101-volt"];
	}
	catch(err)
	{
		alert('Invalid Miner configuration file. Edit manually or reset to default.');
	}
}
function f_submit_miner_conf() {
	_nexell_pool1url = "stratum+tcp://sg.stratum.slushpool.com:3333";
	_nexell_pool1user = "maekdal.bcworker1";
	_nexell_pool1pw = "123";
	_nexell_pool2url = "stratum+tcp://sg.stratum.slushpool.com:3333";
	_nexell_pool2user = "maekdal.bcworker2";
	_nexell_pool2pw = "123";
	_nexell_pool3url = "stratum+tcp://sg.stratum.slushpool.com:3333";
	_nexell_pool3user = "maekdal.bcworker3";
	_nexell_pool3pw = "123";
	_nexell_nobeeper = "false";
	_nexell_notempoverctrl = "false";
	try
	{
		for(var i = 0; i < nexell_data.pools.length; i++) {
			switch(i) {
			case 0:
				_nexell_pool1url = nexell_data.pools[i].url;
				_nexell_pool1user = nexell_data.pools[i].user;
				_nexell_pool1pw = nexell_data.pools[i].pass;
				break;
			case 1:
				_nexell_pool2url = nexell_data.pools[i].url;
				_nexell_pool2user = nexell_data.pools[i].user;
				_nexell_pool2pw = nexell_data.pools[i].pass;
				break;
			case 2:
				_nexell_pool3url = nexell_data.pools[i].url;
				_nexell_pool3user = nexell_data.pools[i].user;
				_nexell_pool3pw = nexell_data.pools[i].pass;
				break;
			}
		}
		if(nexell_data["miner_nobeeper"]) {
			_nexell_nobeeper = "true";
		} else {
			_nexell_nobeeper = "false";
		}
		if(nexell_data["miner_notempoverctrl"]) {
			_nexell_notempoverctrl = "true";
		} else {
			_nexell_notempoverctrl = "false";
		}
	}
	catch(err)
	{
		alert('Invalid Miner configuration file. Edit manually or reset to default.'+err);
	}
	
	_nexell_cutoff=jQuery("#nexell_cutoff").val();
	_nexell_voltage=jQuery("#nexell_voltage").val();
	
	jQuery("#cbi_apply_cgminer_fieldset").show();
	
	jQuery.ajax({
		url: '/cgi-bin/set_miner_conf.cgi',
		type: 'POST',
		dataType: 'json',
		timeout: 30000,
		cache: false,
		data: {_nexell_pool1url:_nexell_pool1url, _nexell_pool1user:_nexell_pool1user, _nexell_pool1pw:_nexell_pool1pw,_nexell_pool2url:_nexell_pool2url, _nexell_pool2user:_nexell_pool2user, _nexell_pool2pw:_nexell_pool2pw,_nexell_pool3url:_nexell_pool3url, _nexell_pool3user:_nexell_pool3user, _nexell_pool3pw:_nexell_pool3pw, _nexell_nobeeper:_nexell_nobeeper, _nexell_notempoverctrl:_nexell_notempoverctrl, _nexell_cutoff:_nexell_cutoff, _nexell_voltage:_nexell_voltage},
		success: function(data) {
			window.location.reload();
		},
		error: function() {
			window.location.reload();
		}
	});
}

jQuery(document).ready(function() {
	f_get_miner_conf();
});
</script>
<title>Nexell BitMiner</title>
</head>
<body class="lang_en">
	<p class="skiplink">
		<span id="skiplink1"><a href="#navigation">Skip to navigation</a></span>
		<span id="skiplink2"><a href="#content">Skip to content</a></span>
	</p>
	<div id="menubar">
		<h2 class="navigation"><a id="navigation" name="navigation">Navigation</a></h2>
		<div class="clear"></div>
	</div>
	<div id="menubar" style="background-color: #0a2b40;">
		<div class="hostinfo" style="float: left; with: 500px;">
			<img src="/images/Nexell-logo-White_Transpraency_50-1.png" width="178" height="18" alt="" title="" border="0" />
		</div>
		<div class="clear"></div>
	</div>
	<div id="maincontainer">
		<div id="tabmenu">
			<div class="tabmenu1">
				<ul class="tabmenu l1">
					<li class="tabmenu-item-status"><a href="/index.html">System</a></li>
					<li class="tabmenu-item-system active"><a href="/cgi-bin/minerConfiguration.cgi">Miner Configuration</a></li>
					<li class="tabmenu-item-network"><a href="/cgi-bin/minerStatus.cgi">Miner Status</a></li>
					<li class="tabmenu-item-system"><a href="/network.html">Network</a></li>
				</ul>
				<br style="clear: both" />
				<div class="tabmenu2">
					<ul class="tabmenu l2">
						<li class="tabmenu-item-system"><a href="/cgi-bin/minerConfiguration.cgi">General Settings</a></li>
						<li class="tabmenu-item-system active"><a href="/cgi-bin/minerAdvanced.cgi">Advanced Settings</a></li>
					</ul>
					<br style="clear: both" />
				</div>
			</div>
		</div>
		<div id="maincontent">
			<noscript>
				<div class="errorbox">
					<strong>Java Script required!</strong><br /> You must enable Java Script in your browser or LuCI will not work properly.
				</div>
			</noscript>
			<h2 style="padding-bottom:10px;"><a id="content" name="content">Miner Advanced Configuration</a></h2>
			<div class="cbi-map" id="cbi-cgminer">
				<fieldset class="cbi-section" id="cbi_msg_cgminer_fieldset" style="display:none">
					<span id="cbi_msg_cgminer" style="color:red;"></span>
				</fieldset>
				<fieldset class="cbi-section" id="cbi_apply_cgminer_fieldset" style="display:none">
					<img src="/resources/icons/loading.gif" alt="Loading" style="vertical-align:middle" />
					<span id="cbi-apply-cgminer-status">Waiting for changes to be applied...</span>
				</fieldset>
				<fieldset class="cbi-section" id="cbi-cgminer-cgminer">
					<div class="cbi-section-descr"></div>
					<fieldset class="cbi-section" id="cbi-cgminer-default">
						<legend>Settings</legend>
						<div class="cbi-value" id="cbi-cgminer-default-cutoff">
							<label class="cbi-value-title" for="cbid.cgminer.default.cutoff">Cutoff Temperature</label>
							<div class="cbi-value-field">
								<input type="text" class="cbi-input-text" name="cbid.cgminer.default.cutoff" id="nexell_cutoff" value="" />
								<span style="color:red;">Modify Cutoff Temperature, clkick Save&Apply, and then re-power the miner.</span>
							</div>
						</div>
						<div class="cbi-value" id="cbi-cgminer-default-voltage">
							<label class="cbi-value-title" for="cbid.cgminer.default.pool3url">Voltage</label>
							<div class="cbi-value-field">
								<select id="nexell_voltage" class="cbi-input-text">
									<option value="400">0.40V</option>
									<option value="420">0.42V</option>
								</select>
								<span style="color:red;">Select voltage, clkick Save&Apply, and then re-power the miner.</span>
							</div>
						</div>
					</fieldset>
					<br />
				</fieldset>
				<br />
			</div>
			<div class="cbi-page-actions">
				<input class="cbi-button cbi-button-save right" type="button" onclick="f_submit_miner_conf();" value="Save&Apply" />
				<input class="cbi-button cbi-button-reset right" type="button" onclick="f_get_miner_conf();" value="Reset" />
			</div>
			<div class="clear"></div>
		</div>
	</div>
	<div class="clear"></div>
	<div style="text-align: center; bottom: 0; left: 0; height: 1.5em; font-size: 80%; margin: 0; padding: 5px 0px 2px 8px; background-color: #918ca0; width: 100%;">
		<font style="color:#fff;">Copyright &copy; 2009-2018, Nexell Technologies</font>
	</div>
</body>
</html>
EOT
