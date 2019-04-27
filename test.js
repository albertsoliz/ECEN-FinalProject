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
function grab_enc_pass(user_id,acc_password){
	//console.log("grabbing enc_pass");
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


var aa = "sample_id1";var a = "sample2@gmail.com";var b = "sample_pass2";
var bb = "sample_pass3";var c = "2345";var d = 1234; var e = "log";
var ee = "9999";
//write_NewUser(aa,a,b,c,d,e);
//update_enc_pass(aa,ee);
grab_enc_pass(aa,b);
var path_to_exec = "/home/bitnami/progs/a.out";
var arg1 = "12384";
var arg2 = "454545";
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
