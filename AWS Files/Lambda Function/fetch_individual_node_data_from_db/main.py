import json
import boto3
from boto3.dynamodb.conditions import Key


def lambda_handler(event, context):
    # 1. Connecting to Database
    client = boto3.resource('dynamodb')
    table = client.Table('complete_bpcl_database')

    # 2. Parse Query String Params
    print(event)
    cmd = event['queryStringParameters']['cmd']
    print(f"cmd: {cmd}")
    epoch_time = int(event['queryStringParameters']['epoch_time'])

    if((cmd == 'live') or (cmd == 'hist') or (cmd == 'avg') or (cmd == 'csv')):  # for fetching historical data
        nodeid = event['queryStringParameters']['nodeid']
    elif (cmd == 'combined_avg'):
        nodes = int(event['queryStringParameters']['nodes'])

    # 3. Process the request
    if(cmd == 'live'):  # For fetching the latest time
        dt_count = epoch_time - (10*60*1000)
    elif ((cmd == 'hist') or (cmd == 'avg') or (cmd == 'csv') or (cmd == 'combined_avg')):
        time_period = int(event['queryStringParameters']['time_period'])
        if(time_period == 8):  # 8 hour historical data
            dt_count = epoch_time - (8*60*60*1000)
        elif (time_period == 24):  # 24 hour historical data
            dt_count = epoch_time - (24*60*60*1000)
        elif (time_period == 7):  # 7 days historical data
            dt_count = epoch_time - (7*24*60*60*1000)
        elif (time_period == 1):  # 1 month historical data
            dt_count = epoch_time - (30*24*60*60*1000)

    if((cmd == 'live') or (cmd == 'hist') or (cmd == 'avg') or (cmd == 'csv')):
        query_response = table.query(KeyConditionExpression=Key(
            'nodeid').eq(nodeid) & Key('dt').gte(dt_count))
        items = query_response['Items']

    # 4. Interpret Answer and Construct the Response
    response = {}
    response['cmd'] = cmd
    response['nodeid'] = nodeid

    if(cmd == 'live'):
        if items:
            last_dt = 0
            return_item = {}
            for item in items:
                if(item['dt'] > last_dt):
                    last_dt = item['dt']
                    return_item = item
            response['pm_1'] = int(return_item['pm_1'])
            response['pm_2_5'] = int(return_item['pm_2_5'])
            response['pm_10'] = int(return_item['pm_10'])
            response['dt'] = int(return_item['dt'])
        else:
            response['pm_1'] = None
            response['pm_2_5'] = None
            response['pm_10'] = None
            response['dt'] = None

    elif(cmd == 'hist'):
        if items:
            response['hist_data'] = []
            last_dt = 0
            pm1 = 0
            pm2_5 = 0
            pm10 = 0
            counts = 1
            increments = 0
            time_interval = int((epoch_time-dt_count)/11)
            first_dt = 0
            first_epoch = 0
            for item in items:
                if(last_dt == 0):
                    last_dt = item['dt']
                    first_dt = last_dt
                    time_upper_count = (counts*time_interval)+first_dt
                if (time_upper_count-last_dt < 0):
                    if increments != 0:
                        param_dic['pm_1'] = int(pm1/increments)
                        param_dic['pm_2_5'] = int(pm2_5/increments)
                        param_dic['pm_10'] = int(pm10/increments)
                        response['hist_data'].append(param_dic)
                        counts += 1
                        increments = 0
                        pm1 = 0
                        pm2_5 = 0
                        pm10 = 0
                        time_upper_count = int(counts*time_interval)+first_dt
                        first_epoch = 0
                else:
                    if(first_epoch == 0):
                        param_dic = {}
                        param_dic['dt'] = int(item['dt'])
                        first_epoch = 1
                    pm1 += item['pm_1']
                    pm2_5 += item['pm_2_5']
                    pm10 += item['pm_10']
                    last_dt = item['dt']
                    increments += 1
            response['data_points'] = counts-1
        else:
            response['data_points'] = None

    elif(cmd == 'csv'):
        if items:
            response['csv_data'] = []
            counts = 0
            for item in items:
                param_dic = {}
                param_dic['pm_1'] = int(item['pm_1'])
                param_dic['pm_2_5'] = int(item['pm_2_5'])
                param_dic['pm_10'] = int(item['pm_10'])
                param_dic['dt'] = int(item['dt'])
                response['csv_data'].append(param_dic)
                counts += 1
            response['data_points'] = counts
        else:
            response['data_points'] = None

    elif(cmd == 'avg'):
        if items:
            pm1 = 0
            pm2_5 = 0
            pm10 = 0
            data_points = len(items)
            for item in items:
                pm1 += item['pm_1']
                pm2_5 += item['pm_2_5']
                pm10 += item['pm_10']
            pm1 = pm1/data_points
            pm2_5 = pm2_5/data_points
            pm10 = pm10/data_points
            response['pm_1'] = int(pm1)
            response['pm_2_5'] = int(pm2_5)
            response['pm_10'] = int(pm10)
            response['dt'] = int((dt_count + epoch_time)/2)
        else:
            response['pm_1'] = None
            response['pm_2_5'] = None
            response['pm_10'] = None
            response['dt'] = None

    elif(cmd == 'combined_avg'):
        avg_pm1 = 0
        avg_pm2_5 = 0
        avg_pm10 = 0
        fetched_nodes = 0
        inactive_nodes = []
        for i in range(nodes):
            query_response = table.query(KeyConditionExpression=Key(
                'nodeid').eq(event['queryStringParameters'][f'nodeid_{i}']) & Key('dt').gte(dt_count))
            items = query_response['Items']
            pm1 = 0
            pm2_5 = 0
            pm10 = 0
            increments = 0
            for item in items:
                pm1 += item['pm_1']
                pm2_5 += item['pm_2_5']
                pm10 += item['pm_10']
                increments += 1
            if (increments != 0):
                fetched_nodes += 1
                avg_pm1 += (pm1/increments)
                avg_pm2_5 += (pm2_5/increments)
                avg_pm10 += (pm10/increments)
            else:
                inactive_nodes.append(
                    event['queryStringParameters'][f'nodeid_{i}'])
        if (fetched_nodes != 0):
            avg_pm1 = avg_pm1/fetched_nodes
            avg_pm2_5 = avg_pm2_5/fetched_nodes
            avg_pm10 = avg_pm10/fetched_nodes
            response['pm_1'] = int(avg_pm1)
            response['pm_2_5'] = int(avg_pm2_5)
            response['pm_10'] = int(avg_pm10)
            response['inactive_nodes'] = inactive_nodes
        else:
            response['pm_1'] = None
            response['pm_2_5'] = None
            response['pm_10'] = None

    # 3. Construct http response object
    responseObject = {}
    responseObject['statusCode'] = 200
    responseObject['headers'] = {
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Credentials": "true",
        "Access-Control-Allow-Headers": "Origin,Content-Type,X-Amz-Date,Authorization,X-Api-Key,X-Amz-Security-Token,locale",
        "Access-Control-Allow-Methods": "POST, OPTIONS",
        "content-type": "application/json"
    }
    responseObject['body'] = json.dumps(response)

    return responseObject
