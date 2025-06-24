from pyais import decode

mensagem = "!AIVDM,1,1,,A,15MvqP0000G?tTpN>jd4QwvN0<0r,0*4D"

mensagemDecodificada = decode(mensagem)

parametros = mensagemDecodificada.asdict()

for chave, valor in parametros.items():
    print(f"{chave}: {valor}")