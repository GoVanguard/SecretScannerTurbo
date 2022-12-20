Secret Scanner Turbo

Searches for common keys and secrets in a stupidly simple way.

Setup:
 - Git clone repo
 - Cd into directory
 - chmod a+x searchTexts

Usage:
./searchTexts /your/path/


Example Output 1:
./searchTexts ./fixtures

./fixtures//aws.json:9:15:     "aws_id": "AKIAHI38FAKE1IWUQEEN",
./fixtures//aws.xml:10:12:     <aws_id>AKIAHI38FAKE1IWUQEEN</aws_id>
./fixtures//aws.yml:7:8: aws_id: AKIAHI38FAKE1IWUQEEN
./fixtures//severity.yml:2:10:   aws_id: AKIAHI38FAKE1IWUQEEN
./fixtures//aws.json:13:0: ASIArewfgwergf
./fixtures//.npmrc:8:68: //artifactory.jfrog.com/artifactory/api/npm/npm/:_authToken=eyFake1.eyJFake2.Fake3.hardcoded
./fixtures//falsepositive.yml:7:8:   secretSecretary: "ok6"
./fixtures//falsepositive.yml:12:12:   webhookApiSecret: ZENDESK_WEBHOOK_API_SECRET
./fixtures//falsepositive.yml:18:11:   secretarySecret: "hardcoded1"
./fixtures//.npmrc:1:27: //registry.npmjs.org/:_authToken=$NPM_TOKEN
./fixtures//.npmrc:2:27: //registry.npmjs.org/:_authToken=${NPM_TOKEN}
./fixtures//.npmrc:3:27: //registry.npmjs.org/:_authToken={NPM_TOKEN}
