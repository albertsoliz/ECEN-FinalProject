var admin = require("firebase-admin");
var execFile = require('child_process').execFile
var serviceAccount = require("/home/bitnami/progs/FirebaseInstall/database/key.json");
var fs = require('fs');
var passwordHash = require('password-hash');

var inputPass =  'password123';
var hashedPassword = passwordHash.generate(inputPass);
console.log("hasehd password: "+ hashedPassword);
console.log("Verify pass: "+ passwordHash.verify(inputPass,hashedPassword) );

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://database-16835.firebaseio.com"
});

var root_db = admin.database();
var usersRef = root_db.ref('users');

//console.log("print all users");
usersRef.once("value").then( function(snapshot) {
	console.log("Snapshot of: "+ usersRef + "  is " + snapshot.val());
	var a = snapshot.exists();//
	console.log("Snapshot existence?:" + a);
	console.log("snapshot value:" + JSON.stringify(snapshot.val()) );
	}, function(errorObject){
  		console.log("Read failed: " + errorObject.code);
});

//add new user file
function write_NewUser(user_id,email,password,enc,hwid,logs){
	var new_pass = passwordHash.generate(password);
	console.log("Writing new user");
	 root_db.ref('users/'+user_id).set( {
		acc_email: email,
		acc_password: new_pass,
		enc_pass: enc,
		hw_id: hwid,
		log:logs,
		user_id: user_id
	}, function(error){
		if(error) {console.log(error);}
		else{console.log("added new user");}
	//var newUserKey = firebase.database().ref().child('users' + email).setValue(newUser);
	});
}

//hash user password
function hash_pass(password){
	var hashed_password = password;

	console.log("hashed pass");
	return hashed_password;
}


function update_enc_pass(user_id,new_enc_pass){
	root_db.ref('users/'+user_id).update( {
		enc_pass: new_enc_pass
	},function(error){
		if(error) {console.log(error);}
		else{console.log("updated enc_pass");}
	});
}

function update_log(user_id,new_log){
	root_db.ref('users/'+user_id).update( {
		log: new_log
	},function(error){
		if(error) {console.log(error);}
		else{console.log("updated log");}
	});
}

//grab encrypted password string
function grab_enc_pass(user_id,acc_password){
	console.log("grabbing enc_pass");
	var pass = passwordHash.generate(acc_password);//redundant
	root_db.ref('users/'+user_id).once('value').then(function(snapshot) {
		var enc_pass = snapshot.val().enc_pass;
		var user_pass = snapshot.val().acc_password;
		if(passwordHash.verify(acc_password,user_pass) ){
			console.log("grabbing enc_pass");
			console.log(enc_pass);
			return enc_pass;
		}
		else{console.log("wrong password");}
		//console.log( JSON.stringify(snapshot.val().enc_pass) );
	}, function(error){ 
		if(error){console.log(error);}
		else{console.log("grabbed enc_pass");}
	});	
}

//return log
function grab_log(user_id,acc_password){
	console.log("grabbing log");
	var pass = passwordHash.generate(acc_password);//redundant
	root_db.ref('users/'+user_id).once('value').then(function(snapshot) {
		var log = snapshot.val().log;
		var user_pass = snapshot.val().acc_password;
		if(passwordHash.verify(acc_password,user_pass) ){
			console.log("grabbing log");
			console.log(log);
			return log;
		}
		else{console.log("wrong password");}
		//console.log( JSON.stringify(snapshot.val().enc_pass) );
	}, function(error){ 
		if(error){console.log(error);}
		else{console.log("grabbed log");}
	});	
}

var aa = "sample_id1";var a = "sample2@gmail.com";var b = "sample_pass2";
var bb = "sample_pass3";var c = "2345";var d = 1234; var e = "log";
var ee = "9999";
//write_NewUser(aa,a,b,c,d,e);
//update_enc_pass(aa,ee);
grab_enc_pass(aa,b);
var path_to_exec = "/home/bitnami/progs/RSA";
//TODO "/home/bitnami/progs/ELGAMAL";
var arg1 = "e021757c777288dacfe67cb2e59dc02c70a8cebf56262336592c18dcf466e0a4ed405318ac406bd79eca29183901a557db556dd06f7c6bea175dcb8460b6b1bc05832b01eedf86463238b7cb6643deef66bc4f57bf8ff7ec7c4b8a8af14f478980aabedd42afa530ca47849f0151b7736aa4cd2ff37f322a9034de791ebe3f51";
var arg2 = "ed1571a9e0cd4a42541284a9f98b54a6af67d399d55ef888b9fe9ef76a61e892c0bfbb87544e7b24a60535a65de422830252b45d2033819ca32b1a9c4413fa721f4a24ebb5510ddc9fd6f4c09dfc29cb9594650620ff551a62d53edc2f8ebf10beb86f483d463774e5801f3bb01c4d452acb86ecfade1c7df601cab68b065275";
var output = "888888";
var child = execFile(path_to_exec, [arg1, arg2],
  function (error, stdout, stderr) {
    // This callback is invoked once the child terminates
    // You'd want to check err/stderr as well!
	if(error){console.log("failed to spawn child process");console.log(error);}
	else{	
	child.on('exit',function(){ process.exit()})//wait
    	fs.readFile('/home/bitnami/progs/output.txt','utf8',function(err,data){
		if(err) throw err;
		//console.log(data);
		output = data;
		console.log("OUTPUT is now: " + output);
		update_enc_pass(aa,output);
	});
	}
});

//update_enc_pass(aa,output);

//wait 1.5 seconds then create new user
setTimeout(function() {
	var id = "sample_id2";
	var email = "sample2@gmail.com";
	var password = "password2" ;
	var encrypt = " ";
	var hw_id = 4455;
	var logs = " ";
	 write_NewUser(id,email,password,encrypt,hw_id,logs );
} , 1500);//1.5 second delay then run function

//wait 5secs then update logs and enc_password fields
setTimeout(function() {
	var id = "sample_id2";
	var encrypt = "173865465454544465465465465465454654654654654654654654654";
	var logs = "Sample log, no action required.";
	 update_enc_pass(id,encrypt );
	 update_log(id,logs);
} , 5000);//5 second delay then run function

//wait 10 seconds then return the new_users enc_pass & logs
setTimeout(function() {
	var id = "sample_id2";
	var pass = "password2";
 grab_enc_pass(id,pass);
 grab_log(id,pass);
	 console.log("Grabbed user log is: ");
	 console.log("Grabbed user enc_pass is : ");
} , 10000);//10 second delay then run function

//wait 15 seconds then return the new_users enc_pass & logs
setTimeout(function() {
	var id = "sample_id2";
	var pass = "password3";
console.log("Attempting to access data with WRONG password");
 grab_enc_pass(id,pass);
 grab_log(id,pass);
	// console.log("Grabbed user log is: ");
	// console.log("Grabbed user enc_pass is : ");
} , 15000);//15 second delay then run function


