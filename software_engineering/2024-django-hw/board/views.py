import json
from django.http import HttpRequest, HttpResponse

from board.models import Board, User
from utils.utils_request import BAD_METHOD, request_failed, request_success, return_field
from utils.utils_require import MAX_CHAR_LENGTH, CheckRequire, require
from utils.utils_time import get_timestamp
from utils.utils_jwt import generate_jwt_token, check_jwt_token

@CheckRequire
def startup(req: HttpRequest):
    return HttpResponse("Congratulations! You have successfully installed the requirements. Go ahead!")


@CheckRequire
def login(req: HttpRequest):
    if req.method != "POST":
        return BAD_METHOD
    
    # Request body example: {"userName": "Ashitemaru", "password": "123456"}
    body = json.loads(req.body.decode("utf-8"))
    
    username = require(body, "userName", "string", err_msg="Missing or error type of [userName]")
    password = require(body, "password", "string", err_msg="Missing or error type of [password]")
    
    try:
        user = User.objects.filter(name=username).first()
        if user:
            if user.password == password:
                return request_success({"token": generate_jwt_token(username)})
            else:
                return request_failed(2, "Wrong password", 401)
        else:
            User(name=username,password=password).save()
            return request_success({"token": generate_jwt_token(username)})
    except Exception as e:
        return request_failed(-4, str(e), 500)
    finally:
        pass


def check_for_board_data(body):
    board = require(body, "board", "string", err_msg="Missing or error type of [board]")
    board_name = require(body, "boardName", "string", err_msg="Missing or error type of [boardName]")
    user_name = require(body, "userName", "string", err_msg="Missing or error type of [userName]")
    assert 0 < len(board_name) <= 50, "Bad length of [boardName]"
    assert 0 < len(user_name) <= 50, "Bad length of [userName]"
    assert len(board) == 2500, "Bad length of [board]"
    assert all(char in ['0', '1'] for char in board), "Invalid char in [board]"
    
    return board, board_name, user_name


@CheckRequire
def boards(req: HttpRequest):
    if req.method == "GET":
        params = req.GET
        boards = Board.objects.all().order_by('-created_time')
        return_data = {
            "boards": [
                # Only provide required fields to lower the latency of
                # transmitting LARGE packets through unstable network
                return_field(board.serialize(), ["id", "boardName", "createdAt", "userName"]) 
            for board in boards],
        }
        return request_success(return_data)
        
    
    elif req.method == "POST":
        jwt_token = req.headers.get("Authorization")
        body = json.loads(req.body.decode("utf-8"))
        try:
            data = check_jwt_token(jwt_token)
            if data == None:
                return request_failed(2, "Invalid or expired JWT", 401)
            
            board, board_name, user_name = check_for_board_data(body)
            jwt_username = require(data, "username", "string")
            if jwt_username != user_name:
                return request_failed(3, "Permission denied", 403)
            else:
                user = User.objects.filter(name=user_name).first()
                if Board.objects.filter(board_name=board_name,user=user).exists():
                    Board.objects.filter(board_name=board_name,user=user).update(board_state=board)
                    return request_success({"isCreate":False})
                else:
                    
                    Board(board_state=board,board_name=board_name,user=user).save()
                    return request_success({"isCreate":True})
                
        except ValueError as e:
            return request_failed(-2, str(e), 400)
        except Exception as e:
            return request_failed(-4, str(e), 500)
    else:
        return BAD_METHOD


@CheckRequire
def boards_index(req: HttpRequest, index: any):
    
    idx = require({"index": index}, "index", "int", err_msg="Bad param [id]", err_code=-1)
    assert idx >= 0, "Bad param [id]"
    
    if req.method == "GET":
        params = req.GET
        board = Board.objects.filter(id=idx).first()  # Return None if not exists
        
        if board:
            return request_success(
                return_field(board.serialize(), ["board", "boardName", "userName"])
            )
            
        else:
            return request_failed(1, "Board not found", status_code=404)
    
    elif req.method == "DELETE":
        try:
            jwt_token = req.headers.get("Authorization")
            data = check_jwt_token(jwt_token)
            if data == None:
                return request_failed(2, "Invalid or expired JWT", 401)
            jwt_username = require(data, "username", "string")
            board = Board.objects.filter(id=idx).first()
            if board == None:
                return request_failed(1, "Board not found", status_code=404)
            else:
                if board.user.name != jwt_username:
                    return request_failed(3, "Cannot delete board of other users", 403)
                else:
                    Board.objects.filter(id=idx).delete()
                    return request_success()
        except Exception as e:
            return request_failed(-4, str(e), 500)
    
    else:
        return BAD_METHOD


@CheckRequire
def user_username(req:HttpRequest, userName: any):
    assert 0 < len(userName) <= 50, "Bad param [userName]"
    if req.method == "GET":
        try:
            user = User.objects.filter(name=userName).first()
            if user:
                user = User.objects.filter(name=userName).first()
                boards = Board.objects.filter(user=user)
                return request_success(data={
                        "userName": userName,
                        "boards": [
                            return_field(board.serialize(), ["id", "boardName", "createdAt", "userName"]) 
                            for board in boards]
                    })
            else:
                return request_failed(1, "User not found", 404)
        except Exception as e:
            return request_failed(-4, str(e), 500)
    else:
        return BAD_METHOD
